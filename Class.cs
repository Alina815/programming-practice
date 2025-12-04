using Microsoft.Data.Sqlite;
using System.Globalization;

public class TariffException : ArgumentException
{
    public TariffException(string message) : base(message) { }
}

public interface IPriceStrategy
{
    double CalculatePrice(double price);
}

public class NoDiscountStrategy : IPriceStrategy
{
    public double CalculatePrice(double price) => price;
}

public class DiscountStrategy : IPriceStrategy
{
    public double Discount { get; }

    public DiscountStrategy(double discount)
    {
        if (discount < 0 || discount > 100)
            throw new TariffException("скидка должна быть вещественным числом в диапазоне от 0 до 100");
        Discount = discount;
    }
    public double CalculatePrice(double price) => price - price * Discount / 100.0;
}

public abstract class Tariff
{ 
    public double Price { get; }
    public string City1 { get; }
    public string City2 { get; }

    protected Tariff(double price, string c1, string c2)
    {
        if (price < 0)
            throw new TariffException("цена не может быть отрицательной");
        Price = price;
        City1 = c1;
        City2 = c2;
    }

    public abstract double GetPrice();
}

public class UsualTariff : Tariff
{
    public readonly NoDiscountStrategy Strategy = new NoDiscountStrategy();
    public UsualTariff(double price, string c1, string c2) : base(price, c1, c2) { }
    override public double GetPrice() => Strategy.CalculatePrice(Price);
}

public class DiscountTariff : Tariff
{
    public double Discount;
    public readonly DiscountStrategy Strategy;
    public DiscountTariff(double price, double discount, string c1, string c2) : base(price, c1, c2)
    {
        Strategy = new DiscountStrategy(discount);
        Discount = discount;
    }
    override public double GetPrice() => Strategy.CalculatePrice(Price);
}

public class ATE
{
    private readonly List<UsualTariff> UsualTariffs = new List<UsualTariff>();
    private readonly List<DiscountTariff> DiscountTariffs = new List<DiscountTariff>();
    public void AddUsualTariff(double p, string c1, string c2)
    {
        UsualTariffs.Add(new UsualTariff(p, c1, c2));
    }
    public void AddDiscountTariff(double p, double d, string c1, string c2)
    {
        DiscountTariffs.Add(new DiscountTariff(p, d, c1, c2));
    }
    public double CalculateAveragePrice()
    {
        if (UsualTariffs.Count == 0 && DiscountTariffs.Count == 0)
            throw new TariffException("списки тарифов пусты");
        return UsualTariffs.Sum(l => l.GetPrice()) + DiscountTariffs.Sum(l => l.GetPrice()) / (double)(UsualTariffs.Count + DiscountTariffs.Count);
    }
    public void SetLists(List<string> tariff_list)
    {
        UsualTariffs.Clear();
        DiscountTariffs.Clear();
        foreach (string line in tariff_list)
        {
            string[] parts = line.Split(";");
            if (parts.Length == 4 && parts[0] == "u")
            {
                string c1 = parts[1];
                string c2 = parts[2];
                double p = double.Parse(parts[3], CultureInfo.InvariantCulture);
                UsualTariffs.Add(new UsualTariff(p, c1, c2));
            }
            else if (parts.Length == 5 && parts[0] == "d")
            {
                string c1 = parts[1];
                string c2 = parts[2];
                double p = double.Parse(parts[3], CultureInfo.InvariantCulture);
                double d = double.Parse(parts[4], CultureInfo.InvariantCulture);
                DiscountTariffs.Add(new DiscountTariff(p, d, c1, c2));
            }
            else
            {
                throw new TariffException("неверный формат строки в списке тарифов");
            }
        }
    }
    public List<string> GetLists()
    {
        var tariff_list = new List<string>();
        foreach (var t in UsualTariffs)
            tariff_list.Add($"u;{t.City1};{t.City2};{t.Price.ToString("F2", CultureInfo.InvariantCulture)}");
        foreach (var t in DiscountTariffs)
            tariff_list.Add($"d;{t.City1};{t.City2};{t.Price.ToString("F2", CultureInfo.InvariantCulture)};{t.Discount.ToString("F2", CultureInfo.InvariantCulture)}");
        return tariff_list;
    }
    public List<string> SortByPrice(List<string> tariff_list)
    {
        return tariff_list.OrderBy(line => double.Parse(line.Split(';')[3], CultureInfo.InvariantCulture)).ToList();
    }
    public List<string> SortByCity1(List<string> tariff_list)
    {
        return tariff_list.OrderBy(line => line.Split(';')[1]).ToList();
    }
    public List<string> SortByCity2(List<string> tariff_list)
    {
        return tariff_list.OrderBy(line => line.Split(';')[2]).ToList();
    }

    public string dbPath;
    public bool dbMode { get; set; }

    public bool InitializeDatabase(string db_path)
    {
        try
        {
            dbPath = db_path;
            using var connection = new SqliteConnection($"Data Source={dbPath}");
            connection.Open();
            using var command = connection.CreateCommand();
            command.CommandText = @"
            CREATE TABLE IF NOT EXISTS Tariffs (
                Id INTEGER PRIMARY KEY AUTOINCREMENT,
                City1 TEXT NOT NULL,
                City2 TEXT NOT NULL,
                Price REAL NOT NULL,
                Discount REAL,
                Type TEXT CHECK(Type IN ('Usual', 'Discount')) NOT NULL
            )";
            command.ExecuteNonQuery();
            dbMode = true;
            return true;
        }
        catch
        {
            dbMode = false;
            return false;
        }
    }

    public void SaveToDatabase()
    {
        if (!dbMode) return;
        using var connection = new SqliteConnection($"Data Source={dbPath}");
        connection.Open();
        using (var command = connection.CreateCommand())
        {
            command.CommandText = "DELETE FROM Tariffs";
            command.ExecuteNonQuery();
        }
        foreach (var tariff in UsualTariffs)
        {
            using var command = connection.CreateCommand();
            command.CommandText = @"
                INSERT INTO Tariffs (City1, City2, Price, Discount, Type)
                VALUES ($city1, $city2, $price, NULL, 'Usual')";
            command.Parameters.AddWithValue("$city1", tariff.City1);
            command.Parameters.AddWithValue("$city2", tariff.City2);
            command.Parameters.AddWithValue("$price", tariff.Price);
            command.ExecuteNonQuery();
        }
        foreach (var tariff in DiscountTariffs)
        {
            using var command = connection.CreateCommand();
            command.CommandText = @"
                INSERT INTO Tariffs (City1, City2, Price, Discount, Type)
                VALUES ($city1, $city2, $price, $discount, 'Usual')";
            command.Parameters.AddWithValue("$city1", tariff.City1);
            command.Parameters.AddWithValue("$city2", tariff.City2);
            command.Parameters.AddWithValue("$price", tariff.Price);
            command.Parameters.AddWithValue("$discount", tariff.Discount);
            command.ExecuteNonQuery();
        }
    }
    public void LoadFromDatabase()
    {
        if (!dbMode) return;
        UsualTariffs.Clear();
        DiscountTariffs.Clear();
        using var connection = new SqliteConnection($"Data Source={dbPath}");
        connection.Open();
        using var command = connection.CreateCommand();
        command.CommandText = "SELECT City1, City2, Price, Discount, Type FROM Tariffs";
        using var reader = command.ExecuteReader();
        while (reader.Read())
        {
            string city1 = reader.GetString(0);
            string city2 = reader.GetString(1);
            double price = reader.GetDouble(2);
            string type = reader.GetString(4);
            if (type == "Usual")
            {
                UsualTariffs.Add(new UsualTariff(price, city1, city2));
            } else if (type == "Discount")
            {
                double discount = reader.IsDBNull(3) ? 0.0 : reader.GetDouble(3);
                DiscountTariffs.Add(new DiscountTariff(price, discount, city1, city2));
            }
        }
    }
}
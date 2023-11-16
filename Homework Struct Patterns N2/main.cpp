#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>

class DatabaseClient
{
public:
	std::string GetData(std::string& key) noexcept
	{
		return "Very Big Data String: " + key;
	}
};

class CachedDatabaseClient : public DatabaseClient
{
public:
	std::unique_ptr<DatabaseClient> ptr;
	std::unordered_map<std::string, std::string> cache;
	explicit CachedDatabaseClient(std::unique_ptr<DatabaseClient> pClient) : ptr(std::move(pClient)) {}
	std::string GetData(std::string& key) noexcept
	{
		auto it = cache.find(key);
		if (it == cache.end())
		{
			auto data = ptr->GetData(key);
			cache.insert({ key,data });
		}
		return cache[key];
	}
};

class TestDatabaseClient : public DatabaseClient
{
public:
	std::string GetData(std::string& key)
	{
		return "Testing....";
	}
};

class Application : public DatabaseClient
{
	DatabaseClient* client = nullptr;
public:
	void VeryComplexCalculations() {}
};

class OneShotDB : public DatabaseClient
{
	int count;
	int sum;
public:
	std::unique_ptr<DatabaseClient> ptr;
	OneShotDB(std::unique_ptr<DatabaseClient> pClient, int count) : ptr(std::move(pClient)), count(count) { sum = 0; }
	std::string GetData(std::string& key)
	{
		if (sum < count)
		{
			++sum;
			return ptr->GetData(key);
		}
		return "error";
	}
};

int main()
{
	std::string key = "key";
	std::unique_ptr<DatabaseClient> real_db = std::make_unique<DatabaseClient>();
	auto limit_db = OneShotDB(std::move(real_db), 2);
	std::cout << limit_db.GetData(key) << std::endl;
	std::cout << limit_db.GetData(key) << std::endl;
	std::cout << limit_db.GetData(key) << std::endl;

	return 0;
}
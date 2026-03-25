/*
mai multe abonamente la net - mai multe routere - fiecare router cu un abonament la net - calculatoare conectate la routere
fiecare abonament are o limita de trafic - fiecare calculator are un consum care se cumuleaza la routerul pe care il foloseste

interogare: depaseste un router capacitatea de trafic a abonamentului sau? - daca da,
parcurgem calculatoarele de pe router si le scadem traficul cu un procent in functie de abonament, astfel incat
traficul routerului sa nu depaseasca limita impusa de abonament. 
Unele calculatoare sunt servere si nu le poate fi scazut traficul.
Daca scaderea traficului nu ar rezolva problema supraincarcarii, atunci crestem pretul platit de router pentru abonament;

INPUT: (pe cate un rand)
nr abonamente
traficul maxim impus de abonament, procentul cu care va fi scazut traficul calculatoarelor conectate la router, pret initial, cu cat creste pretul per MB depasit dupa reducerea traficului
nr routere
indexul abonamentului fiecarui router
nr calculatoare
traficul consumat de calculator, indexul routerului la care este conectat si daca este sau nu server

OPERATII POSIBILE:
adaugarea dinamica a calculatoarelor pe routere.
monitorizarea consumului total si detectarea supraincarcarilor.
calcularea traficului routerului dupa o eventuala reducere procentuala a traficului permis.
scaderea traficului pe calculatoare client cu exceptia serverelor, daca acesta ar elimina supraincarcarea.
cresterea pretului abonamentului pentru routere supraincarcate.
*/

#include <iostream>
#include <fstream>
#include <string>

class Subscription
{
private:
    int maxTraffic; // MB
    float trafficReductionPercentage;
    int basePrice;
    int priceIncreasePerMB;

public:
    Subscription(int mT = 100, float tRP = 20, int bP = 50, int pI = 10) : maxTraffic(mT), trafficReductionPercentage(tRP) , basePrice(bP), priceIncreasePerMB(pI)
    {
        if (trafficReductionPercentage < 0)
            trafficReductionPercentage = 0;
        if (trafficReductionPercentage > 100)
            trafficReductionPercentage = 100;

        if (priceIncreasePerMB < 0)
            priceIncreasePerMB = 0;
    };

    int getMaxTraffic() const
    {
        return maxTraffic;
    }

    float getTrafficReductionPercentage() const
    {
        return trafficReductionPercentage;
    }

    int getBasePrice() const
    {
        return basePrice;
    }

    int getPriceIncreasePerMB() const
    {
        return priceIncreasePerMB;
    }

    friend std::ostream& operator<<(std::ostream& out, const Subscription& instance) 
    {
        out << "Abonament: Limita trafic: " << instance.maxTraffic << "MB, " << "Pret de baza: " << instance.basePrice << " RON";

        return out;
    }
};

class Computer
{
private:
    int baseTraffic;
    int traffic; // MB
    bool isServer;

public:
    Computer(int t = 0, bool s = false) : traffic(t), isServer(s), baseTraffic(t) {};

    int getTraffic() const
    {
        return traffic;
    }

    int getBaseTraffic() const
    {
        return baseTraffic;
    }

    bool getIsServer() const
    {
        return isServer;
    }

    void limitTraffic(float percentage)
    {
        if (isServer)
            return;

        traffic -= int((percentage / 100.0) * traffic);
    }

    int getTrafficAfterDecrease(float percentage) const
    {
        if (isServer)
            return traffic;

        return traffic - int((percentage / 100.0) * traffic);
    }

    friend std::ostream& operator<<(std::ostream& out, const Computer& instance) 
    {
        out << "Trafic initial: " << instance.baseTraffic << "MB, Trafic actual: " << instance.traffic << "MB | Este server: " << (instance.isServer ? "Da" : "Nu") << "\n";

        return out;
    }
};

class Router
{
private:
    int maxTraffic;
    int usedTraffic;
    const Subscription *subscription;
    Computer *connectedComputers = nullptr;
    int computersCount;
    int computersCapacity;
    int subscriptionCost;

    void increaseComputerCapacity()
    {
        computersCapacity *= 2;
        Computer *newVector = new Computer[computersCapacity];
        for(int i = 0; i < computersCount; i++)
            newVector[i] = connectedComputers[i];
        delete[] connectedComputers;
        connectedComputers = newVector;
    }
    
public:
    explicit Router(Subscription *sub = nullptr) : subscription(sub), maxTraffic(0), usedTraffic(0), 
    computersCount(0), computersCapacity(2), subscriptionCost(0)
    {
        if (subscription != nullptr)
        {
            maxTraffic = subscription->getMaxTraffic();
            subscriptionCost = subscription->getBasePrice();
        }
        connectedComputers = new Computer[2];
    };

    ~Router()
    {
        delete[] connectedComputers;
    };

    Router(const Router &src) : subscription(src.subscription), maxTraffic(src.maxTraffic), 
    usedTraffic(src.usedTraffic), computersCount(src.computersCount), computersCapacity(src.computersCapacity),
    subscriptionCost(src.subscriptionCost)
    {
        connectedComputers = new Computer[computersCapacity]; 

        for(int i = 0; i < computersCount; i++)
        {
            connectedComputers[i] = src.connectedComputers[i];
        }
    }

    Router& operator=(const Router &src)
    {
        if (this == &src)
            return *this;

        delete[] connectedComputers;

        subscription = src.subscription;
        maxTraffic = src.maxTraffic;
        usedTraffic = src.usedTraffic;
        computersCount = src.computersCount;
        computersCapacity = src.computersCapacity;
        subscriptionCost = src.subscriptionCost;

        connectedComputers = new Computer[computersCapacity];
        for(int i = 0; i < computersCount; i++)
        {
            connectedComputers[i] = src.connectedComputers[i];
        }

        return *this;
    }

    int getUsedTraffic() const
    {
        return usedTraffic;
    }

    int getRemainingTraffic() const
    {
        return maxTraffic - usedTraffic;
    }

    int getMaxTraffic() const
    {
        return maxTraffic;
    }

    bool hasTooMuchTraffic() const
    {
        return usedTraffic > maxTraffic;
    }

    void addComputer(const Computer &computer)
    {
        if (computersCount == computersCapacity)
            increaseComputerCapacity();

        connectedComputers[computersCount] = computer;
        computersCount++;

        const int computerTraffic = computer.getTraffic();
        usedTraffic += computerTraffic;
    }

    void updateTraffic()
    {
        usedTraffic = 0;
        for(int i = 0; i < computersCount; i++)
        {
            usedTraffic += connectedComputers[i].getTraffic();
        }
    }

    void limitComputersTraffic()
    {
        float percentage = subscription->getTrafficReductionPercentage();
        for(int i = 0; i < computersCount; i++)
        {
            connectedComputers[i].limitTraffic(percentage);
        }
        updateTraffic();
    }
    
    int getTrafficAfterDecrease() const
    {
        float percentage = subscription->getTrafficReductionPercentage();
        int newTraffic = 0;
        for(int i = 0; i < computersCount; i++)
        {
            newTraffic += connectedComputers[i].getTrafficAfterDecrease(percentage);
        }

        return newTraffic;
    }

    bool shouldLimitComputersTraffic() const
    {
        if (getTrafficAfterDecrease() > maxTraffic)
            return false;
        return true;
    }

    void increaseSubscriptionCost()
    {
        int surplus = usedTraffic - maxTraffic;
        if (surplus <= 0)
            return;

        subscriptionCost += surplus * subscription->getPriceIncreasePerMB();
    }

    const Computer* getConnectedComputers() const
    {
        return connectedComputers;
    }

    friend std::ostream& operator<<(std::ostream& out, const Router& instance) 
    {
        out << "Router:\n";
        out << *(instance.subscription) << ", Trafic consumat: " << instance.usedTraffic << "MB, Pret actual: " << instance.subscriptionCost << " RON\n";

        if (instance.hasTooMuchTraffic())
        {
            out << "Supraincarcat; daca s-ar fi scazut traficul consumat, acesta ar fi fost: " << instance.getTrafficAfterDecrease() << "MB\n";
        }

        if (instance.computersCount > 0)
        {
            out << "Calculatoare conectate: \n";
            for(int i = 0; i < instance.computersCount; i++)
            {
                out << instance.connectedComputers[i];
            }
        }
        
        out << "\n\n";

        return out;
    }
};

int main()
{
    std::ifstream fin("date.txt");
    
    // citire abonamente
    int subscriptionsAmount;
    fin >> subscriptionsAmount;
    Subscription *subscriptions = new Subscription[subscriptionsAmount];

    int maxTraffic, trafficReductionPercentage, cost, costIncrease;
    for(int i = 0; i < subscriptionsAmount; i++)
    {
        fin >> maxTraffic >> trafficReductionPercentage >> cost >> costIncrease;
        subscriptions[i] = Subscription(maxTraffic, trafficReductionPercentage, cost, costIncrease);
    }
    
    // citire routere
    int routersAmount;
    fin >> routersAmount;
    Router *routers = new Router[routersAmount];

    int subscriptionIndex;
    for(int i = 0; i < routersAmount; i++)
    {
        fin >> subscriptionIndex;
        routers[i] = Router(&subscriptions[subscriptionIndex-1]);
    }

    // citire computere
    int computersAmount;
    fin >> computersAmount;
    int traffic, routerIndex;
    bool isServer;
    for(int i = 0; i < computersAmount; i++)
    {
        fin >> traffic >> routerIndex >> isServer;
        Computer c(traffic, isServer);
        routers[routerIndex-1].addComputer(c);
    }

    // functionalitate

    for (int i = 0; i < routersAmount; i++)
    {
        Router &router = routers[i];
        if (router.hasTooMuchTraffic())
        {
            if (router.shouldLimitComputersTraffic())
                router.limitComputersTraffic();

            if (router.hasTooMuchTraffic())
            {
                router.increaseSubscriptionCost();
            }
        }
    }

    // afisare

    for (int i = 0; i < routersAmount; i++)
    {
        std::cout << routers[i];
    }

    // eliberare memorie
    delete[] subscriptions;
    delete[] routers;

    return 0;
}
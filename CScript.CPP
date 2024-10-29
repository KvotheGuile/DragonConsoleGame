#include <iostream>
#include <format>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

void Clear()
{
    cout<<"\033[2J\033[1;1H";
}

void Wait()
{
    cout<<"\n\nPress Enter... ";
    char temp;
    temp = cin.get();
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Clear();
}

int Random(int maxPosible)
{
    srand(time(0));
    return rand() % (maxPosible + 1) - 0;
}


class Character
{
    public:

    string name;
    int maxHp;
    int hp;

    int potions;
    int arrows;

    Character(string _name, int _hp)
    {
        name = _name;
        maxHp = _hp;
        hp = maxHp;
        potions = 0;
        arrows = 0;
    }
    
    void DrawHp()
    {
        cout<<name<<": HP - ["<<hp<<"/"<<maxHp<<"] "<<((int)(100 * (double)hp/maxHp))<<"%"<<endl;
    }

    void Inventory()
    {
       cout<<"Opening inventory"<<endl;
       Wait();
       cout<<"\n"<<name<<"'s Inventory:"<<endl;

       bool hasSomething = arrows > 0 || potions > 0;

       if (hasSomething)
       {
            if (arrows > 0)
            {
                cout<<"- Arrows:   "<<arrows<<endl;
            }
            if (potions > 0)
            {
                cout<<"- Potions:  "<<potions<<endl;
            }
       }else
       {
            cout<<"Inventory is empty"<<endl;
       }
       cout<<endl;

    }

    void TakeDamage(double dmg, int critMod=0)
    {   
        if (Random(8 + critMod) == 0)
        {
            cout<<"\nCRITICAL HIT!!!!\n";
            dmg *= 2;
        }

        hp -= (int)dmg;

        if (hp<=0)
        {
            hp = 0;
        }

        cout<<name<<" loses "<<(int)dmg<<" hp. It is now at "<<hp<<" hp"<<endl;
    }

    bool Attack(Character &target, string weapon)
    {
        double damage = 5 + Random(5);
        int crit = 0;

        if (weapon == "bow")
        {
            if (arrows > 0)
            {
                damage *= 2;
                cout<<"Arrows left: "<<--arrows<<"\n"<<endl;
                crit -= 2;

                if (Random(10) == 0)
                {
                    damage = 0;
                }

            }
            else
            {
                cout<<"No arrows left.\n"<<endl;
                return false;
            }
        }
        else if (weapon == "claw")
        {
            damage += 2;
            crit += 8;
        }
        else if (weapon == "fire breath")
        {
            damage *= 2;
            damage += 10;
            crit += 6;

            if (Random(5) == 0)
            {
                damage = 0;
            }
        }
        else if (weapon == "wing flap")
        {
            damage *= 0.5;
            damage++;
            crit += 2;

            if (Random(5) == 0)
            {
                damage = 0;
            }
        }

        cout<<name<<" uses "<<weapon<<"!"<<endl;
        Wait();
        cout<<name<<" uses "<<weapon<<"!"<<endl;

        if (damage > 0)
        {
            target.TakeDamage(damage, crit);
        }
        else
        {
            cout<<"The attack missed..."<<endl;
        }

        //cout<<"ENEMY HP: "<<target.hp<<endl;

        return true;
    }

    bool Heal()
    {
        if (potions > 0)
        {
            potions--;
            hp += 25;

            if (hp > maxHp)
            {
                hp = maxHp;
            }

            cout<<"Potions left: "<<potions<<endl;
        }
        else
        {
            cout<<"No potions left."<<endl;
            return false;
        }

        return true;
    }
};

class Monster : public Character 
{
    public:
    
    using Character::Character;

    void Act(Character &enemy)
    {
        int r = Random(10);
        string atk = "claw";

        if (r <= 1)
        {
            atk = "fire breath";
        }
        else if (r <= 3)
        {
            atk = "wing flap";
        }

        Attack(enemy, atk);
    }
};


void DrawState(Character c1, Character c2)
{
    c1.DrawHp();
    c2.DrawHp();
}

void Actions(Character c)
{
    cout<<"1. Sword"<<endl;
    
    if (c.arrows > 0)
    {
        cout<<"2. Bow"<<endl;
    }

    if (c.potions > 0)
    {
        cout<<"3. Potion"<<endl;
    }
    
    cout<<"4. Inventory"<<endl;
    cout<<"5. Exit Game"<<endl;
}

int main()
{
    // Variables
    bool gameOn = true;
    bool turnEnd = false;

    Clear();

    // Setting up the characters
    Character player("Player", 100);
    Monster enemy("Dragon", 150);
    player.potions = 5;
    player.arrows = 6;

    // Game Loop
    while (gameOn)
    {   
        int input;
        string secondaryInput;

        turnEnd = true;
            cout<<"\nPlayer Turn!"<<endl;
        DrawState(player, enemy);
        Actions(player);
        cout<<">> ";
        cin>>input;
        switch (input)
        {
            case 1:
                turnEnd = player.Attack(enemy, "sword");
                break;
            case 2:
                turnEnd = player.Attack(enemy, "bow");
                break;
            case 3:
                turnEnd = player.Heal();
                break;
            case 4:
                player.Inventory();
                turnEnd = false;
                break;
            case 5:
                cout<<"Are you sure you want to exit? [y/n]"<<endl;
                cin>>secondaryInput;
                if (secondaryInput == "y")
                {
                    gameOn=false;
                    turnEnd=false;
                }
                break;
            default:
                cout<<"Command not reconized"<<endl;
                break;
        }
        
        DrawState(player, enemy);
        Wait();

        if (turnEnd && enemy.hp > 0)
        {
            //Enemy Turn
            cout<<"\nEnemy Turn!"<<endl;
            enemy.Act(player);
            DrawState(player, enemy);
            Wait();
        }
        Clear();

        if (player.hp <= 0 || enemy.hp <= 0)
        {
            gameOn = false;
        }

    }

    cout<<"Game Ended."<<endl;
    DrawState(player, enemy);
    cout<<"\n"<<endl;
    Wait();
}

#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

double stringToDouble(const string &str)
{
    double result = 0.0;
    bool decimalPoint = false;
    double decimalFactor = 1.0;

    for (char ch : str)
    {
        if (ch == '.')
        {
            decimalPoint = true;
        }
        else if (ch >= '0' && ch <= '9')
        {
            if (decimalPoint)
            {
                decimalFactor *= 0.1;
                result += (ch - '0') * decimalFactor;
            }
            else
            {
                result = result * 10 + (ch - '0');
            }
        }
    }
    return result;
}

// Helper function to convert string to int
int stringToInt(const string &str)
{
    int result = 0;
    for (char ch : str)
    {
        if (ch >= '0' && ch <= '9')
        {
            result = result * 10 + (ch - '0');
        }
    }
    return result;
}

struct player_game_node
{
    string game_id;
    float hours_played;
    int achievements_unlocked;
    player_game_node *next;
    player_game_node(string id, float hp, int ach)
    {
        game_id = id;
        hours_played = hp;
        achievements_unlocked = ach;
        next = nullptr;
    }
};
struct player_node
{
    string player_id;
    string name;
    string phone_no;
    string email;
    string password;
    player_node *right;
    player_node *left;
    player_game_node *games_played;
    player_node(string id, string n, string phone, string mail, string pass)
    {
        player_id = id;
        name = n;
        phone_no = phone;
        email = mail;
        password = pass;
    }
};
struct game_node
{
    string game_id;
    string name;
    string developer;
    string publisher;
    float file_size_in_GB;
    int downloads;
    game_node *left;
    game_node *right;

    game_node(string id, string n, string dev, string pub, float size, int down)
    {
        game_id = id;
        name = n;
        developer = dev;
        publisher = pub;
        file_size_in_GB = size;
        downloads = down;
        left = right = nullptr;
    }
};
class gamess
{
    game_node *root;

public:
    gamess()
    {
        root = nullptr;
    }
    int seed_generator()
    {
        int seed = 232572; // Replace with your actual seed

        srand(seed);                 // Initialize random seed
        int randNum = rand() % 1001; // Generate a random number from 0 to 1000

        // Get the last two digits of the seed number
        int lastTwoDigits = (seed % 100);

        // Calculate the comparison value
        int compareValue = (lastTwoDigits * 10) + 100;

        // Return the result of comparison if needed
        if (randNum < compareValue)
        {
            return randNum;
        }
        else
            return -1;
    }
    void insert_game(game_node *&node, game_node *new_game)
    {
        if (node == nullptr)
        {
            node = new_game;
            return;
        }

        if (new_game->game_id < node->game_id)
            insert_game(node->left, new_game);
        if (new_game->game_id > node->game_id)
            insert_game(node->right, new_game);
    }

    void insert_game(game_node *new_game)
    {
        insert_game(root, new_game);
    }
    void readfile()
    {
        ifstream file;
        file.open("file.csv");

        if (!file)
        {
            cout << "Error opening file." << endl;
            return;
        }

        string line;
        while (getline(file, line))
        {
            string id, name, dev, pub, size, down;
            int i = 0;
            int var_no = 0;
            while (i < line.length())
            {
                string value;
                while (i < line.length() && line[i] != ',')
                {
                    value += line[i];
                    i++;
                }
                i++;

                switch (var_no)
                {
                case 0:
                    id = value;
                    break;
                case 1:
                    name = value;
                    break;
                case 2:
                    dev = value;
                    break;
                case 3:
                    pub = value;
                    break;
                case 4:
                    size = value;
                    break;
                case 5:
                    down = value;
                    break;
                default:
                    break;
                }

                var_no++;
            }

            float file_size = stringToDouble(size);
            int downloads = stringToInt(down);
            game_node *new_game = new game_node(id, name, dev, pub, file_size, downloads);
            insert_game(new_game);
        }

        file.close();
    }
    void preorderTraversal(game_node *node) const
    {
        if (node == nullptr)
            return;

        cout << "Game ID: " << node->game_id << ", Name: " << node->name
             << ", Developer: " << node->developer << ", Publisher: " << node->publisher
             << ", File Size (GB): " << node->file_size_in_GB << ", Downloads: " << node->downloads << endl;

        preorderTraversal(node->left);

        preorderTraversal(node->right);
    }
    void preorderTraversal() const
    {
        cout << "Preorder Traversal of Game Tree:" << endl;
        preorderTraversal(root);
    }
};
int main()
{
    gamess obj;
    obj.readfile();
    obj.preorderTraversal();
}
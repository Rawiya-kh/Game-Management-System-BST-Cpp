
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
        int seed = 232572;

        srand(seed);
        int randNum = rand() % 1001;
        int lastTwoDigits = (seed % 100);
        int compareValue = (lastTwoDigits * 10) + 100;

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

    game_node *search_game(game_node *node, const string &game_id)
    {
        if (node == nullptr || node->game_id == game_id)
            return node;

        if (game_id < node->game_id)
            return search_game(node->left, game_id);
        else
            return search_game(node->right, game_id);
    }

    game_node *search_game(const string &game_id)
    {
        return search_game(root, game_id);
    }

    game_node *delete_game(game_node *node, const string &game_id)
    {
        if (node == nullptr)
            return node;

        if (game_id < node->game_id)
            node->left = delete_game(node->left, game_id);
        else if (game_id > node->game_id)
            node->right = delete_game(node->right, game_id);
        else
        {
            // Case 1
            if (node->left == nullptr && node->right == nullptr)
            {
                delete node;
                node = nullptr;
            }
            // Case 2
            else if (node->left == nullptr)
            {
                game_node *temp = node;
                node = node->right;
                delete temp;
            }
            else if (node->right == nullptr)
            {
                game_node *temp = node;
                node = node->left;
                delete temp;
            }
            // cas 3
            else
            {
                game_node *temp = minValueNode(node->right);
                node->game_id = temp->game_id;
                node->name = temp->name;
                node->developer = temp->developer;
                node->publisher = temp->publisher;
                node->file_size_in_GB = temp->file_size_in_GB;
                node->downloads = temp->downloads;

                node->right = delete_game(node->right, temp->game_id);
            }
        }
        return node;
    }

    void delete_game(const string &game_id)
    {
        root = delete_game(root, game_id);
    }

    game_node *minValueNode(game_node *node)
    {
        game_node *current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }
    void saveToFile(game_node *node, ofstream &file)
    {
        if (node == nullptr)
            return;

        file << node->game_id << "," << node->name << "," << node->developer << ","
             << node->publisher << "," << node->file_size_in_GB << "," << node->downloads << endl;

        saveToFile(node->left, file);
        saveToFile(node->right, file);
    }

    void saveToFile()
    {
        ofstream file;
        file.open("games_data.csv");

        if (!file)
        {
            cout << "Error opening file for saving." << endl;
            return;
        }

        saveToFile(root, file);

        file.close();
    }
};
int main()
{
    gamess obj;
    obj.readfile();
    string search_id = "3468322375";
    game_node *found_game = obj.search_game(search_id);
    if (found_game)
        cout << "Game found: " << found_game->name << endl;
    else
        cout << "Game not found!" << endl;

    string delete_id = "3468322375";
    obj.delete_game(delete_id);
    cout << "Game with ID " << delete_id << " deleted." << endl;

    obj.preorderTraversal();
    obj.saveToFile();
    cout << "Data saved to CSV file!" << endl;
}

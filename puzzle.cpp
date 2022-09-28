
#include <bits/stdc++.h>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


class puzzle
{
    private:
        string player;
        Mat image;
        int mul[10][2] = {{2, 2}, {2, 1}, {2, 1}, {2, 1}, {2, 1}, {1, 2}, {1, 1}, {1, 1}, {1, 1}, {1, 1}};
        vector<pair<bool, pair<int, int>>> make_a_choice = {make_pair(false, make_pair(-1, -1)), make_pair(false, make_pair(-1, -1)), make_pair(false, make_pair(-1, -1))}; // 2x1 1x2 1x1
    public:
        vector<pair<int, int>> empty_boxes;
        vector<pair<int, int>> boxes;
        pair<int, int> process_input(int x, int y);
        void set_to_white(pair<int, int> c1, pair<int, int> c2);
        void modify_boxes(int x, int y);
        void create_image_from_boxes();
        void find_empty_spaces();
        void game_play();
        puzzle(const string player_name);
};


void locator(int event, int x, int y, int flags, void* userdata);


/*
Starting position
   0   1   2   3
0 [|] [*] [*] [|]
1 [|] [*] [*] [|]
2 [ ] [-] [-] [ ]
3 [|] [^] [^] [|]
4 [|] [<] [>] [|]
boxes: (always keeps left top coordinate)
    2x2, 2x1, 1x2, 1x1
*/


void puzzle::create_image_from_boxes()
{
    // init image
    Mat img(500, 400, CV_8UC3, Scalar(0,0, 0));
    image = img;
    int colours[10][3] = {{252, 98, 3}, {148, 3, 252}, {148, 3, 252}, {148, 3, 252}, {148, 3, 252}, {3, 252, 140}, {36, 3, 252}, {36, 3, 252}, {36, 3, 252}, {36, 3, 252}};
    for (int i = 0; i < 10; i++)
    {
        // Top Left Corner
        Point p1(((boxes[i].second)*100), ((boxes[i].first)*100));

        // Bottom Right Corner
        Point p2(((boxes[i].second)*100+100*mul[i][1]), ((boxes[i].first)*100+100*mul[i][0]));

        int thickness = -1;

        // Drawing the Rectangle
        rectangle(image, p1, p2,
                Scalar(colours[i][0], colours[i][1], colours[i][2]),
                thickness, LINE_8);

        // Radius of circle
        int radius = 10;

        Point p3(((boxes[i].second)*100+50*mul[i][1]), ((boxes[i].first)*100+50*mul[i][0]));

        circle(image, p3, radius, Scalar(0, 0, 0), thickness);
    }
}


// function to track mouse movement and click
void locator(int event, int x, int y, int flags, void* userdata)
{
    puzzle* obj = reinterpret_cast<puzzle*>(userdata);
    // when left button clicked
    if (event == EVENT_LBUTTONDOWN)
    {
        pair<int, int> temp = obj->process_input(x/100, y/100);
        // take action after input and change boxes or not
        if (temp.first != -1)   // -1 is flag for invalid input to change anything
            obj->modify_boxes(temp.first, temp.second);
    }
}


void puzzle::set_to_white(pair<int, int> c1, pair<int, int> c2)
{
    for (int i = 0; i < 2; i++)
    {
        // Top Left Corner
        Point p1(((empty_boxes[i].second)*100), ((empty_boxes[i].first)*100));

        // Bottom Right Corner
        Point p2(((empty_boxes[i].second)*100+100), ((empty_boxes[i].first)*100+100));

        int thickness = -1;

        // Drawing the Rectangle
        rectangle(image, p1, p2, Scalar(255, 255, 255), thickness, LINE_8);
    }
}


pair<int, int> puzzle::process_input(int x, int y)
{
    // every rectangle has a ref point
    // any mouse click on any rectangle should converted to ref point of that rectangle
    for (int i = 0; i < 10; i++)
    {
        auto x_y = boxes[i];
        if (i == 0)
        {
            if (x == x_y.second && y == x_y.first)
                return make_pair(y, x);
            else if (x == x_y.second+1 && y == x_y.first)
                return make_pair(y, x-1);
            else if (x == x_y.second && y == x_y.first+1)
                return make_pair(y-1, x);
            else if (x == x_y.second+1 && y == x_y.first+1)
                return make_pair(y-1, x-1);
        }
        else if (i == 1 || i == 2 || i == 3 || i == 4)
        {
            if (x == x_y.second && y == x_y.first)
                return make_pair(y, x);
            else if (x == x_y.second && y == x_y.first+1)
                return make_pair(y-1, x);
        }
        else if (i == 5)
        {
            if (x == x_y.second && y == x_y.first)
                return make_pair(y, x);
            else if (x == x_y.second+1 && y == x_y.first)
                return make_pair(y, x-1);
        }
        else
        {
            if (x == x_y.second && y == x_y.first)
                return make_pair(y, x);
        }
    }

// if square is white
    int t1 = image.at<Vec3b>(100*y+50, 100*x+20)[0];//getting the pixel values//
    int t2 = image.at<Vec3b>(100*y+50, 100*x+20)[1];//getting the pixel values//
    int t3 = image.at<Vec3b>(100*y+50, 100*x+20)[2];//getting the pixel values//
    if (t1 == 255 && t2 == 255 && t3 == 255)
        return make_pair(y*10+10, x*10);
// no changes
    return make_pair(-1, -1);
}


void puzzle::find_empty_spaces()
{
    empty_boxes.clear();
    int horizontal[4] = {20, 120, 220, 320};
    int vertical[5] = {50, 150, 250, 350, 450};
    for (int h = 0; h < 4; h++)
    {
        for (int v = 0; v < 5; v++)
        {
            int t1 = image.at<Vec3b>(vertical[v], horizontal[h])[0];//getting the pixel values//
            int t2 = image.at<Vec3b>(vertical[v], horizontal[h])[1];//getting the pixel values//
            int t3 = image.at<Vec3b>(vertical[v], horizontal[h])[2];//getting the pixel values//
            if (t1 == 0 && t2 == 0 && t3 == 0)
                empty_boxes.push_back(make_pair(v, h));
        }
    }
}


void puzzle::modify_boxes(int x, int y)
{
    find_empty_spaces();    // fills empty_boxes to black square's matrix indexes
    bool choice = false;
    bool flag = false;

// if a choice is made with white squares
    if (x >= 10)
    {
        x -= 10;
        x /= 10;
        y /= 10;

        // see where make_a_choice is generated for better understanding
        if (make_a_choice[0].first)
        {
            if (x == make_a_choice[0].second.first+3  && y == make_a_choice[0].second.second)
                x --;
            boxes[make_a_choice[2].second.first] = make_pair(x, y);
        }

        else if (make_a_choice[1].first)
        {
            if (x == make_a_choice[1].second.first  && y == make_a_choice[1].second.second+3)
                y --;
            boxes[make_a_choice[2].second.first] = make_pair(x, y);
        }

        else
            boxes[make_a_choice[2].second.first] = make_pair(x, y);

        flag = true;
    }

    // reset
    make_a_choice = {make_pair(false, make_pair(-1, -1)), make_pair(false, make_pair(-1, -1)), make_pair(false, make_pair(-1, -1))}; // 2x1 1x2 1x1
    // create image based on boxes array
    create_image_from_boxes();    

    if (flag)
        return;

    int temp = 0;
    // check if clicked on any rectangle
    for (int i = 0; i < 10; i++)
    {
        auto x_y = boxes[i];
        if (i == 0 && x == x_y.first && y == x_y.second)    // 2x2
        {
            // (X-1, Y) -if-> (X-1, Y) and (X-1, Y+1) is empty and on matrix range
            if (x-1 >= 0 && y+1 <= 3 && (empty_boxes[0].first == x-1 && empty_boxes[0].second == y) && (empty_boxes[1].first == x-1 && empty_boxes[1].second == y+1))
                boxes[i] = make_pair(x-1, y);

            // (X+1, Y) -if-> (X+2, Y) ile (X+2, Y+1) is empty and on matrix range
            else if (x+2 <= 4 && y+1 <= 3 && (empty_boxes[0].first == x+2 && empty_boxes[0].second == y) && (empty_boxes[1].first == x+2 && empty_boxes[1].second == y+1))
                boxes[i] = make_pair(x+1, y);

            // (X, Y-1) -if-> (X, Y-1) ile (X+1, Y-1) is empty and on matrix range
            else if (x+1 <= 4 && y-1 >= 0 && (empty_boxes[0].first == x && empty_boxes[0].second == y-1) && (empty_boxes[1].first == x+1 && empty_boxes[1].second == y-1))
                boxes[i] = make_pair(x, y-1);

            // (X, Y+1) -if-> (X, Y+2) ile (X+1, Y+2) is empty and on matrix range
            else if (x+1 <= 4 && y+2 <= 3 && (empty_boxes[0].first == x && empty_boxes[0].second == y+2) && (empty_boxes[1].first == x+1 && empty_boxes[1].second == y+2))
                boxes[i] = make_pair(x, y+1);

            else        // if there is no possible_moves then skip and don't modify boxes
                continue;
        }


        else if ((i == 1 || i == 2 || i == 3 || i == 4) && x == x_y.first && y == x_y.second)   // 2x1
        {
            // (X-1, Y) -if-> (X-1, Y) is empty and on matrix range
            if (x-1 >= 0 && y <= 3 && ((empty_boxes[0].first == x-1 && empty_boxes[0].second == y) || (empty_boxes[1].first == x-1 && empty_boxes[1].second == y)))
                temp = 1;

            // (X+1, Y) -if-> (X+2, Y) is empty and on matrix range
            if (x+2 <= 4 && y <= 3 && ((empty_boxes[0].first == x+2 && empty_boxes[0].second == y) || (empty_boxes[1].first == x+2 && empty_boxes[1].second == y)))
                if (temp == 1)
                    temp = 3;
                else
                    temp = 2;

            if (temp != 0)
                temp = temp;

            // (X, Y+1) -if-> (X, Y+1) ile (X+1, Y+1) is empty and on matrix range
            else if (x+1 <= 4 && y+1 <= 3 && (empty_boxes[0].first == x && empty_boxes[0].second == y+1) && (empty_boxes[1].first == x+1 && empty_boxes[1].second == y+1))
                boxes[i] = make_pair(x, y+1);

            // (X, Y-1) -if-> (X, Y-1) ile (X+1, Y-1) is empty and on matrix range
            else if (x+1 <= 4 && y-1 <= 3 && (empty_boxes[0].first == x && empty_boxes[0].second == y-1) && (empty_boxes[1].first == x+1 && empty_boxes[1].second == y-1))
                boxes[i] = make_pair(x, y-1);

            else        // if there is no possible_moves then skip and don't modify boxes
                continue;


            if (temp == 3)
            {
                // (X-1, Y) is chosen on next move modify boxes[i] to (X-1, Y)
                // (X+2, Y) is chosen on next move modify boxes[i] to (X+1, Y)
                make_a_choice[0] = make_pair(true, make_pair(x-1, y));
                make_a_choice[2] = make_pair(false, make_pair(i, i));   // to save i
                set_to_white(make_pair(x-1, y), make_pair(x+2, y));
                choice = true;
            }

            else if (temp == 1)
                boxes[i] = make_pair(x-1, y);

            else if (temp == 2)
                boxes[i] = make_pair(x+1, y);
        }


        else if (i == 5 && x == x_y.first && y == x_y.second)   // 1x2
        {
            // (X, Y+1) -if-> (X, Y+2) is empty and on matrix range
            if (x >= 0 && y+2 <= 3 && ((empty_boxes[0].first == x && empty_boxes[0].second == y+2) || (empty_boxes[1].first == x && empty_boxes[1].second == y+2)))
                temp = 1;

            // (X, Y-1) -if-> (X, Y-1) is empty and on matrix range
            if (x <= 4 && y-1 >= 0 && ((empty_boxes[0].first == x && empty_boxes[0].second == y-1) || (empty_boxes[1].first == x && empty_boxes[1].second == y-1)))
                if (temp == 1)
                    temp = 3;
                else
                    temp = 2;

            if (temp != 0)
                temp = temp;

            // (X-1, Y) -if-> (X-1, Y) ile (X-1, Y+1) is empty and on matrix range
            else if (x-1 >= 0 && y+1 <= 3 && (empty_boxes[0].first == x-1 && empty_boxes[0].second == y) && (empty_boxes[1].first == x-1 && empty_boxes[1].second == y+1))
                boxes[i] = make_pair(x-1, y);

            // (X+1, Y) -if-> (X+1, Y) ile (X+1, Y+1) is empty and on matrix range
            else if (x+1 <= 4 && y+1 <= 3 && (empty_boxes[0].first == x+1 && empty_boxes[0].second == y) && (empty_boxes[1].first == x+1 && empty_boxes[1].second == y+1))
                boxes[i] = make_pair(x+1, y);

            else        // if there is no possible_moves then skip and don't modify boxes
                continue;


            if (temp == 3)
            {
                // (X, Y+2) is chosen on next move modify boxes[i] to (X, Y+1)
                // (X, Y-1) is chosen on next move modify boxes[i] to (X, Y-1)
                make_a_choice[1] = make_pair(true, make_pair(x, y-1));
                make_a_choice[2] = make_pair(false, make_pair(i, i));   // to save i
                set_to_white(make_pair(x, y+2), make_pair(x, y-1));
                choice = true;
            }

            else if (temp == 1)
                boxes[i] = make_pair(x, y+1);

            else if (temp == 2)
                boxes[i] = make_pair(x, y-1);
        }


        else if ((i == 6 || i == 7 || i == 8 || i == 9) && x == x_y.first && y == x_y.second)   // 1x1
        {
            vector<pair<int, int>> temp = {make_pair(x-1, y), make_pair(x+1, y), make_pair(x, y-1), make_pair(x, y+1)};
            vector<pair<int, int>> possible_moves;
            // find possible moves for 1x1 square
            for (int l = 0; l < 4; l++)
                if (((empty_boxes[0].first == temp[l].first && empty_boxes[0].second == temp[l].second) || (empty_boxes[1].first == temp[l].first && empty_boxes[1].second == temp[l].second)))
                    possible_moves.push_back(temp[l]);

            if (possible_moves.size() == 2)
            {
                make_a_choice[2] = make_pair(true, make_pair(i, i));   // to save i
                set_to_white(possible_moves[0], possible_moves[1]);
                choice = true;
            }

            else if (possible_moves.size() == 1)
                boxes[i] = possible_moves[0];
        }


        else
            continue;
    }


// if not squares are set to white
    if (!choice)
        // create image based on boxes array
        create_image_from_boxes();
}


void puzzle::game_play()
{
    create_image_from_boxes();
    // show image and take mouse click as an input from player
    namedWindow("Puzzle");//declaring window to show image//
    setMouseCallback("Puzzle", locator, this);//Mouse callback function on define window//
    while (true)
    {
        imshow("Puzzle", image);
        if (waitKey(10) == 27)   // boxes[0] != make_pair(0, 1) can be added
            break;
    }
    destroyAllWindows();
    cout << "\nCONGRATS\n";
}


puzzle::puzzle(string player_name)
{
    player = player_name;
    // init boxes
    int init_values[10][2] = {{3, 1}, {0, 0}, {0, 3}, {3, 0}, {3, 3}, {2, 1}, {0, 1}, {1, 1}, {0, 2}, {1, 2}};
    for (auto i : init_values)
        boxes.push_back(make_pair(i[0], i[1]));
}


int main()
{
    string player_name_input = "LOSER"; // string()
    // cout << "\nEnter your name : ";
    // getline(cin, player_name_input);
    puzzle puzzle1(player_name_input);
    puzzle1.game_play();
    return 0;
}

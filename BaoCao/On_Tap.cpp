#include <SFML/Graphics.hpp>
#include <time.h>
#include<iostream>
using namespace sf;
using namespace std;


int kt = 54; // kích thước của mỗi viên

struct vienhinh
{
    int x, y, cot, hang, loai, match, alpha; //, nối, nổ alpha 
    vienhinh()
    {
        match = 0;
        alpha = 255;
    }

} vien[9][9];
/*sử dụng một lưới 2 chiều lưu trữ 8 viên ở vị trí cột và hàng trong khung
  trò chơi..mỗi quân có 1 hình ảnh piece(kích thước size*size)*/

void swap(vienhinh p1, vienhinh p2) //hàm đổi chổ hai viên
{
    swap(p1.cot, p2.cot);
    swap(p1.hang, p2.hang);

    vien[p1.hang][p1.cot] = p1; //mỗi viên trong có tọa độ là hàng và cột
    vien[p2.hang][p2.cot] = p2;

}

int Play() {
    // tạo cửa sổ hiển thị chơi game
    RenderWindow app(VideoMode(740, 480), "Game!");
    Vector2i offset(48, 24);
    // Sử dụng vectơ offset() điều chỉnh khi chọn, di chuyển.
    // khoảng trống được căn từ lề vào đối với chiều cao và chiều rộng.

    app.setFramerateLimit(60);
    /*Giới hạn tốc độ khung hình ở một tần số cố định tối đa đảm bảo
    rằng khung hiện tại tồn tại đủ lâu để khớp với giới hạn tốc độ khung*/

    //load các file hình ảnh từ project vào
    Texture t1, t2;
    t1.loadFromFile("images/hinh3.jpg");
    t2.loadFromFile("images/keo66.jpg");

    //các Sprite chứa hình ảnh, có thể di chuyển(mỗi viên có 1 sprite)
    Sprite nen(t1), icon(t2);

    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
        {
            vien[i][j].loai = rand() % 3;//random tạo ra tọa độ ngẫu nhiên của các viên 
            vien[i][j].cot = j; //độ rộng về chiều ngang của viên 
            vien[i][j].hang = i; //độ rộng về chiều rộng của viên 
            vien[i][j].x = j * kt;//kích thước chiều ngang của viên 
            vien[i][j].y = i * kt;// kích thước chiều rộng của viên
        }

    int x0, y0, x, y;
    int click = 0;
    Vector2i pos;
    bool isSwap = false, isMoving = false;

    while (app.isOpen())//vòng lặp game: cả trò chơi là 1 vòng lặp
    {
        Event e;//sử dụng để bắt sự kiện click,close
        while (app.pollEvent(e))
        {
            // Yêu cầu đóng cửa sổ
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::MouseButtonPressed) //Nhấn một nút chuột
                if (e.key.code == Mouse::Left) // kiểm tra nút chuột trái
                {
                    if (!isSwap && !isMoving)
                    {
                        click++;
                    }
                    pos = Mouse::getPosition(app) - Vector2i(offset);// 48 24
                    //vitri chuot khi click
                }
        }

        // nhấp chuột
        if (click == 1)//không hợp lệ: không tính
        {
            x0 = pos.x / kt + 1;
            y0 = pos.y / kt + 1;
        }
        if (click == 2)
        {
            x = pos.x / kt + 1;
            y = pos.y / kt + 1;
            if (abs(x - x0) + abs(y - y0) == 1)
            {
                swap(vien[y0][x0], vien[y][x]);//hợp lệ, thực hiện đổi chỗ các viên
                isSwap = 1;
                click = 0;
            }
            else click = 1;
        }

        /// kết hợp tìm kiếm
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
            {
                if (vien[i][j].loai == vien[i + 1][j].loai)
                    if (vien[i][j].loai == vien[i - 1][j].loai)
                        for (int n = -1; n <= 1; n++)
                            vien[i + n][j].match++;

                if (vien[i][j].loai == vien[i][j + 1].loai)
                    if (vien[i][j].loai == vien[i][j - 1].loai)
                        for (int n = -1; n <= 1; n++)
                            vien[i][j + n].match++;
            }

        //chuyển động các viên hình
        isMoving = false;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
            {
                vienhinh& p = vien[i][j];
                int dx, dy;
                for (int n = 0; n < 4; n++)   // 4 - speed
                {
                    dx = p.x - p.cot * kt;
                    dy = p.y - p.hang * kt;
                    if (dx) p.x -= dx / abs(dx);
                    if (dy) p.y -= dy / abs(dy);
                }
                if (dx || dy) isMoving = 1;
            }

        //xóa các viên
        if (!isMoving)
            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++)
                    if (vien[i][j].match)
                        if (vien[i][j].alpha > 10)
                        {
                            vien[i][j].alpha -= 10;
                            isMoving = true;
                        }

        //Nhận điểm
        int diem = 0;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
                diem += vien[i][j].match;

        //Second swap if no match
        if (isSwap && !isMoving)
        {
            if (!diem)
                swap(vien[y0][x0], vien[y][x]);
            isSwap = 0;
        }

        //cập nhật lưới (bổ sung, cải biến, xóa bớt)
        if (!isMoving)
        {
            for (int i = 8; i > 0; i--)
                for (int j = 1; j <= 8; j++)
                    if (vien[i][j].match)
                        for (int n = i; n > 0; n--)
                            if (!vien[n][j].match)
                            {
                                swap(vien[n][j], vien[i][j]);
                                break;
                            }

            for (int j = 1; j <= 8; j++)
                for (int i = 8, n = 0; i > 0; i--)
                    if (vien[i][j].match)
                    {
                        vien[i][j].loai = rand() % 7;
                        vien[i][j].y = -kt * n++;
                        vien[i][j].match = 0;
                        vien[i][j].alpha = 255;
                    }
        }


        //kéo ra
        app.draw(nen);

        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
            {
                vienhinh p = vien[i][j];
                icon.setTextureRect(IntRect(p.loai * 49, 0, 49, 49));
                icon.setColor(Color(255, 255, 255, p.alpha));
                icon.setPosition(p.x, p.y);
                icon.move(offset.x - kt, offset.y - kt);
                app.draw(icon);
            }

        app.display(); //hiển thị, phải có hàm này mới xem được
    }
    return 0;
}
int Menu()
{
    RenderWindow window(VideoMode(640, 960), "Bejeweled Game");
    window.setFramerateLimit(60);
    Texture t3;
    t3.loadFromFile("images/menu.jpg");
    Sprite backround(t3);
  
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::EventType::Closed:
                window.close();
                break;
            case Event::MouseButtonPressed:
                break;
            }
        }
        window.clear();
        window.draw(backround);

        if (Mouse::isButtonPressed(Mouse::Left)&& Mouse::getPosition(window).x >= 220
            &&Mouse::getPosition(window).y >= 330 && Mouse::getPosition(window).x <= 450
            && Mouse::getPosition(window).y <= 410)
        {
            Play();
        }
        else if (Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(window).x >= 220
            && Mouse::getPosition(window).y >= 500 && Mouse::getPosition(window).x <= 450
            && Mouse::getPosition(window).y <= 560)
        {
            window.close();
        }
        else
            window.draw(backround);
            window.display();
    }
    return 0;
}

int main()
{
    srand(time(0));
    Menu();
    return 0;
}

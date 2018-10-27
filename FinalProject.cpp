#include <stdio.h>
#include <windows.h>
#include <graphics.h>
#include <time.h>

FILE *fp, *fEvent, *fd;
char *month[12]={"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY","AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};
char *week[7]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri","Sat"};
int day[12]={31,28,31,30,31, 30,31,31,30,31,30,31};
char s[30];
int mode = 0, mod = 0;

int GetStartingDay(int Y)
{
    int D=(((Y-1)*365)+ ((Y-1)/4) - ((Y-1)/100)+ ((Y)/400)+1)%7;
    return D;
}

void Today()
{
    int WD;
    time_t CurrentTime;
    time(&CurrentTime);

    struct tm *TIME = localtime(&CurrentTime);
    moveto(1250,1);

    WD=TIME->tm_wday;
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);

    setcolor(LIGHTMAGENTA);
    outtext("TODAY : ");
    sprintf(s, "%i", TIME->tm_mday);
    outtext(s);
    sprintf(s,"/ %i", TIME->tm_mon+1);
    outtext(s);
    sprintf(s, "/ %i ", TIME->tm_year+1900);
    outtext(s);
    outtext(week[WD]);
}

int calender(int year)
{
    int Month, i, j, Day, WeekDay=0,StartDay=0, posY=0, posX=100;
    int d, m, Ed, Em, Ey;
    time_t CurrentTime;
    time(&CurrentTime);
    struct tm *TIME = localtime(&CurrentTime);

    setcolor(CYAN);
    StartDay=GetStartingDay(year);

    if ((year%4==0 && year%100!=0) || year%400==0)
        day[1]=29;
    else
        day[1]=28;

    setcolor(RED);
    cleardevice();
    Today();
    settextstyle(BOLD_FONT, HORIZ_DIR , 5);
    setcolor(LIGHTCYAN);
    sprintf(s,"YEAR : %d",year);
    outtextxy(600, 10, s);


    for (Month=0;Month<12;Month++)
    {
        settextstyle(8, HORIZ_DIR , 3);

        if (Month==4 || Month == 8)
        {
            posX+=500;
            posY=0;
        }

        setcolor(LIGHTGREEN);

        sprintf(s,"\n\t  %s\n",month[Month]);
        outtextxy(55+posX, 60+(Month%4)*200+posY, s);
        setcolor(MAGENTA);

        settextstyle(8, 0 , 1);

        for (j=0;j<67;j++)
            outtextxy(5+j*4+posX, 78+(Month%4)*200+posY, "-");

        setcolor(YELLOW);

        for (j=0;j<7;j++)
            outtextxy(5+j*40+posX, 98+(Month%4)*200, week[j]);

        outtext("\n");
        setcolor(LIGHTGREEN);

        for (WeekDay=0; WeekDay<StartDay; WeekDay++)
            outtext("    ");

        for (Day=1,j=StartDay+1; Day<=day[Month];Day++,j++){

            if (j==6)
                setcolor(RED);
            else
                setcolor(LIGHTBLUE);

            fp = fopen("holiday.txt", "r");

            if (mode == 1)
            {
                while (!feof(fp)){
                    fscanf(fp, "%d", &d);
                    fscanf(fp, "%d", &m);

                    if (Day==d && Month==m){
                        setcolor(RED);
                        break;
                    }
                }
            }
            fclose(fp);

            if (Day==TIME->tm_mday && Month==TIME->tm_mon && year == TIME->tm_year+1900){
                setcolor(WHITE);
            }

            fEvent = fopen("Event.txt", "r");

            while (!feof(fEvent))
            {
                fscanf(fEvent, "%d", &Ed);
                fscanf(fEvent, "%d", &Em);
                fscanf(fEvent, "%d", &Ey);

                if ((Day == Ed) && (Month == Em-1) && (year == Ey))
                {
                    setcolor(12);
                    break;
                }
            }
            rewind(fEvent);
            fclose(fEvent);

            sprintf(s,"%d ",Day);
            outtextxy(-30+j*40+posX, 115+(Month%4)*200+posY, s);

            if (Day==TIME->tm_mday && Month==TIME->tm_mon && year == TIME->tm_year+1900){
                setcolor(WHITE);
                rectangle(-35+j*40+posX, 115+(Month%4)*200+posY, j*40+posX, 129+(Month%4)*200+posY);
            }

            WeekDay++;

            if (WeekDay>6)
            {
                posY+=15;
                WeekDay=0;
                j=0;
            }
            StartDay=WeekDay;
        }
        posY=0;
    }
    setcolor(WHITE);
    outtextxy(50, 800, "-> for NEXT YEAR");
    outtextxy(350, 800, "<- for PREVIOUS YEAR");
    outtextxy(650, 800, "RSHIFT to select YEAR");
    outtextxy(950, 800, "esc to EXIT");
    outtextxy(50, 900, "tab to create new event");
    outtextxy(350, 900, "LCTRL to show all events");

    if (mode == 0)
        outtextxy(1250, 800, "SPACE to show holidays");
    else
        outtextxy(1250, 800, "SPACE to hide holidays");
}



void LoadingScreen()
{
    int p,q,T=0,k=0;

    setcolor(RED);
    settextstyle(BOLD_FONT, HORIZ_DIR, 5);
    outtextxy(getmaxx()/2-350, getmaxy()/2, "LOADING... Please WAIT!!!");

    moveto(125,220);

    bar(getmaxx()/2-350, getmaxy()/2+60, getmaxx()/2+270, getmaxy()/2+80);
    setfillstyle(SOLID_FILL, LIGHTMAGENTA);
    floodfill(getmaxx()/2-345,getmaxy()/2+61, WHITE);

    for (p=0;p<620;p++)
    {
        if (p%5==0)
            delay(1);

        bar(getmaxx()/2-350, getmaxy()/2+60, getmaxx()/2-350+p, getmaxy()/2+80);
        setfillstyle(SOLID_FILL, GREEN);
        floodfill(getmaxx()/2-350+p,getmaxy()/2+61, WHITE);
    }
}

int main()
{
    fp = fopen("holiday.txt", "w");
    fprintf(fp, "21 1 17 2 26 2 14 3 1 4 15 7 16 11 25 11");
    fclose(fp);
    fEvent = fopen("Event.txt", "a");
    fclose(fEvent);

    int year = 2018;
    char respond,nl;
    DWORD ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    initwindow(ScreenWidth, ScreenHeight, "");
    ///setbkcolor(WHITE);
    ///cleardevice();

    Today();
    LoadingScreen();
    calender(year);

    while (1)
    {
        if (GetAsyncKeyState(VK_RIGHT))
        {
            year++;
            calender(year);
        }

        if (GetAsyncKeyState(VK_LEFT))
        {
            year--;
            calender(year);
        }

        if (GetAsyncKeyState(VK_RSHIFT))
        {
            closegraph();
            printf("Enter YEAR : ");
            scanf("%d", &year);
            system("cls");
            initwindow(ScreenWidth, ScreenHeight, "");
            calender(year);
        }

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            closegraph();
            break;
        }

        if (GetAsyncKeyState(VK_SPACE))
        {
            mode = 1 - mode;
            calender(year);
        }


        if (GetAsyncKeyState(VK_TAB))
        {
            int d = -1, m = -1, y = -1;
            char details[50], key;
            closegraph();

            while (1)
            {
                printf("ADD EVENT :\n\n");

                printf("day : ");
                if (d==-1)
                    scanf("%d", &d);
                else
                    printf("%d\n", d);
                if (d>31 || d<0)
                {
                    printf("Invalid day!");
                    Sleep(1000);
                    d = -1;
                    system("cls");
                    continue;
                }

                printf("month : ");
                if (m==-1)
                    scanf("%d", &m);
                else
                    printf("%d\n", m);
                if (m>12 || m<0)
                {
                    printf("Invalid month!\n");
                    Sleep(1000);
                    m = -1;
                    system("cls");
                    continue;
                }

                printf("year : ");
                if (y==-1)
                    scanf("%d", &y);
                else
                    printf("%d\n", y);
                if (y<0)
                {
                    printf("Invalid Year!\n");
                    Sleep(1000);
                    y = -1;
                    system("cls");
                    continue;
                }

                printf("Add event details : ");
                getchar();
                scanf("%[^\n]", details);

                fd = fopen("details.txt", "w");
                fprintf(fd, "%[^\n]", details);
                fclose(fd);
                system("cls");
                printf("CONFIRM EVENT?\nY to confirm\tN to cancel\n");
                getchar();
                scanf("%c", &key);

                if (key=='y' || key=='Y')
                {
                    system("cls");
                    printf("EVENT was created successfully!!!");
                    Sleep(1000);
                    initwindow(ScreenWidth, ScreenHeight, "");
                    fEvent = fopen("Event.txt", "a");
                    fprintf(fEvent, "\n%d %d %d", d, m, y);
                    fclose(fEvent);
                    initwindow(ScreenWidth, ScreenHeight, "");
                    calender(year);
                    system("cls");
                    break;
                }

                if (key=='N' || key=='n')
                {
                    system("cls");
                    printf("EVENT wasn't created!!!");
                    Sleep(1000);
                    d = -1, m = -1, y = -1;
                    system("cls");
                    initwindow(ScreenWidth, ScreenHeight, "");
                    calender(year);
                    break;
                }
            }
        }
    }
}

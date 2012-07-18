/*
------------------------------------------------------
8*10 LED DOTMATRIX SNAKE GAME
------------------------------------------------------
This game could be controlled by two input units:
1>COMPUTER KEYBOARD
2>USING A PHILIPS TV REMOTE
-----------------------------------------------------
DATE:     25/04/2011
-----------------------------------------------------
by Vinod.S <vinodstanur@gmail.com>
-----------------------------------------------------
*/
#include<pic.h>
#define _XTAL_FREQ 20e6
__CONFIG(0x3F3A);
#define out PORTD
#define ms __delay_ms
#define clock RB6=1;RB6=0;
#define reset RB7=1;RB7=0;
#define us __delay_us
unsigned char head[10], body[10], xcord, ycord, XYbuffer[40], food[10], rec = 0, buf = 0, j, p, length = 1, still, s;
unsigned int fdata = 1, speed = 200, i;
void display()
{
    reset;
    for (int s = 0; s < speed; s++) {
        for (i = 0; i < 10; i++) {
            p = 1;
            for (char j = 0; j < 9; j++) {
                if ((s & 0b00000011) == 1) {
                    out = ~((body[i] | head[i]) & p);
                    } else {
                    out = ~((head[i] | food[i]) & p);
                }
                p <<= 1;
            }
            PORTD = 255;
            clock;
        }
    }
}
void pic_init()
{
    TRISD = 0;
    TRISB6 = 0;
    TRISB7 = 0;
    PORTD = 255;
    TRISB0 = 1;
    INTE = 1;
    INTEDG = 0;
    GIE = 1;
    PEIE = 1;
}
void usrt_init()
{
    SYNC = 0;
    BRGH = 1;
    SPEN = 1;
    CREN = 1;
    RX9 = 0;
    SPBRG = 10;
    GIE = 1;
    PEIE = 1;
    RCIE = 1;
    INTF = 0;
}
void head_coordinate()
{
    if (rec == 2) {
        if (ycord != 0) {
            ycord--;
            } else {
            ycord = 9;
        }
        } else if (rec == 8) {
        if (ycord != 9) {
            ycord++;
            } else {
            ycord = 0;
        }
        } else if (rec == 4) {
        if (xcord != 0) {
            xcord--;
            } else {
            xcord = 7;
        }
        } else if (rec == 6) {
        if (xcord != 7) {
            xcord++;
            } else {
            xcord = 0;
        }
    }
}
void timer_init()
{
    T2CON = 0b00000100;
    PR2 = 9;
}
void interrupt usr()
{
    if (INTF == 1)        /////a simple RC5 DECODING////
    {
        PORTD = 255;
        INTF = 0;
        if (RB0 == 0) {
            us(100);
            if (RB0 == 0) {
                us(100);
                if (RB0 == 0) {
                    us(889);
                    if (RB0 == 1) {
                        us(889);
                        if (RB0 == 0) {
                            ms(10.632);
                            buf = 0;
                            for (int xx = 0; xx < 6; xx++) {
                                buf <<= 1;
                                if (RB0 == 0) {
                                    buf++;
                                }
                                ms(1.778);
                            }
                            buf = ~buf;
                            buf &= 0b00111111;
                            if (buf == 3) {
                                speed = (speed / 2);
                                if (speed < 5) {
                                    speed = 5;
                                }
                                } else if (buf == 1) {
                                speed = (speed * 2);
                                if (speed > 5000) {
                                    speed == 5000;
                                }
                                } else {
                                if ((rec != 8) && (buf == 2)) {
                                    rec = 2;
                                }
                                if ((rec != 6) && (buf == 4)) {
                                    rec = 4;
                                }
                                if ((rec != 4) && (buf == 6)) {
                                    rec = 6;
                                }
                                if ((rec != 2) && (buf == 8)) {
                                    rec = 8;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (RCIF == 1)        //////keyboard playing (use w,a,s,d) baud rate 57600)
    {
        RCIF = 0;
        if (rec != 8)
        if (RCREG == 'w') {
            rec = 2;
        }
        if (rec != 6)
        if (RCREG == 'a') {
            rec = 4;
        }
        if (rec != 4)
        if (RCREG == 'd') {
            rec = 6;
        }
        if (rec != 2)
        if (RCREG == 's') {
            rec = 8;
        }
    }
}
////////////to display "out" or any string///////////////////////
void scroll(char a, char b, char c, char d, char e)
{
    short int count = 6;
    while (count > 0) {
        body[0] = ~a;
        a = b;
        b = c;
        c = d;
        d = e;
        e = 255;
        reset;
        for (s = 0; s < still; s++) {
            for (i = 0; i < 10; i++) {
                p = 1;
                for (j = 0; j < 9; j++) {
                    PORTD = ~(body[i] & p);
                    __delay_ms(.1);
                    p <<= 1;
                }
                clock;
            }
        }
        for (i = 9; i > 0; i--) {
            body[i] = body[i - 1];
        }
        count--;
    }
}
////////////////////////font data ///////////////////
void A()
{
    scroll(0xC1, 0xB7, 0x77, 0xB7, 0xC1);
}                //letter A
void B()
{
    scroll(0x01, 0x6D, 0x6D, 0x6D, 0x93);
}
void C()
{
    scroll(0x83, 0x7D, 0x7D, 0x7D, 0xBB);
}
void D()
{
    scroll(0x01, 0x7D, 0x7D, 0xBB, 0xC7);
}
void E()
{
    scroll(0x01, 0x6D, 0x6D, 0x6D, 0x7D);
}
void F()
{
    scroll(0x01, 0x6F, 0x6F, 0x6F, 0x7F);
}
void G()
{
    scroll(0x83, 0x7D, 0x65, 0x6D, 0xA3);
}
void H()
{
    scroll(0x01, 0xEF, 0xEF, 0xEF, 0x01);
}
void I()
{
    scroll(0x7D, 0x7D, 0x1, 0x7D, 0x7D);
}
void J()
{
    scroll(0xF3, 0x7D, 0x7D, 0x03, 0x7F);
}
void K()
{
    scroll(0x01, 0xEF, 0xD7, 0xBB, 0x7D);
}
void L()
{
    scroll(0x01, 0xFD, 0xFD, 0xFD, 0xFD);
}
void M()
{
    scroll(0x01, 0xBF, 0xDF, 0xBF, 0x01);
}
void N()
{
    scroll(0x01, 0xBF, 0xDF, 0xEF, 0x01);
}
void O()
{
    scroll(0x83, 0x7D, 0x7D, 0x7D, 0x83);
}
void P()
{
    scroll(0x01, 0x6F, 0x6F, 0x6F, 0x9F);
}
void Q()
{
    scroll(0x83, 0x7D, 0x75, 0x79, 0x81);
}
void R()
{
    scroll(0x01, 0x6F, 0x6F, 0x6F, 0x91);
}
void S()
{
    scroll(0x9B, 0x6D, 0x6D, 0x6D, 0xB3);
}
void T()
{
    scroll(0x7F, 0x7F, 0x01, 0x7F, 0x7F);
}
void U()
{
    scroll(0x03, 0xFD, 0xFD, 0xFD, 0x03);
}
void V()
{
    scroll(0x07, 0xFB, 0xFD, 0xFB, 0x07);
}
void W()
{
    scroll(0x01, 0xFB, 0xF7, 0xFB, 0x01);
}
void X()
{
    scroll(0x39, 0xD7, 0xEF, 0xD7, 0x39);
}
void Y()
{
    scroll(0x3F, 0xDF, 0xE1, 0xDF, 0x3F);
}
void Z()
{
    scroll(0x79, 0x75, 0x6D, 0x5D, 0x3D);
}                //letter Z
void D0()
{
    scroll(0x83, 0x7D, 0x7D, 0x7D, 0x83);
}                //NUM 0
void D1()
{
    scroll(0xDD, 0xBD, 0x1, 0xFD, 0xFD);
}
void D2()
{
    scroll(0xBD, 0x79, 0x75, 0x6D, 0x9D);
}
void D3()
{
    scroll(0x7B, 0x7D, 0x5D, 0x4D, 0x33);
}
void D4()
{
    scroll(0xCF, 0xAF, 0x6F, 0xEF, 0x1);
}
void D5()
{
    scroll(0xB, 0x5D, 0x5D, 0x5D, 0x63);
}
void D6()
{
    scroll(0x83, 0x6D, 0x6D, 0x6D, 0xB3);
}
void D7()
{
    scroll(0x79, 0x77, 0x6F, 0x5F, 0x3F);
}
void D8()
{
    scroll(0x93, 0x6D, 0x6D, 0x6D, 0x93);
}
void D9()
{
    scroll(0x9B, 0x6D, 0x6D, 0x6D, 0x83);
}                //NUM 9
void SP()
{
    scroll(255, 255, 255, 255, 255);
}                //SPACE
/////////////////////////////////////////
main()
{
    restart:
    pic_init();
    usrt_init();
    timer_init();
    xcord = 3;
    ycord = 4;
    rec = 6;
    food[8] = (0b00001000);
    still = 8;
    length = 1;
    while (1) {
        TMR2ON = 0;
        fdata = (1 << TMR2);
        TMR2ON = 1;
        if (fdata > 0b10000000) {
            fdata = (1 << (TMR2 - 6));
        }
        display();
        XYbuffer[0] = ycord;
        XYbuffer[0] &= 0x0f;
        XYbuffer[0] |= (xcord << 4);
        for (i = length; i > 0; i--) {
            XYbuffer[i] = XYbuffer[i - 1];
        }
        head_coordinate();
        XYbuffer[0] = ycord;
        XYbuffer[0] &= 0x0f;
        XYbuffer[0] |= (xcord << 4);
        for (i = 0; i < 10; i++) {
            if ((XYbuffer[0] & 0x0f) != i) {
                head[i] = 0;
                } else {
                head[i] = (0b10000000 >> (XYbuffer[0] >> 4));
            }
            body[i] = 0;
            for (j = 1; j < (length + 1); j++) {
                if ((XYbuffer[j] & 0x0f) == i) {
                    body[i] |= (0b10000000 >> (XYbuffer[j] >> 4));
                }
            }
            if (head[i] != 0) {
                if (head[i] == food[i]) {
                    TMR2ON = 0;
                    length++;
                    food[i] = 0;
                    food[TMR2] = fdata;
                    TMR2ON = 1;
                }
            }
        }
        if ((head[(XYbuffer[0] & 0x0f)] | body[(XYbuffer[0] & 0x0f)]) == body[(XYbuffer[0] & 0x0f)]) {
            SP();
            O();
            U();
            T();
            SP();
            for (i = 0; i < 300; i++) {
                PORTD = 0;
                ms(.1);
                clock;
            }
            for (i = 0; i <= length; i++) {
                XYbuffer[i] = 0;
                food[i] = 0;
                body[i] = 0;
            }
            length = 0;
            body[0] = 0;
            goto restart;
        }
    }
}
/*PROGRAM END */

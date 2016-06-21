/**/
/*
 delay(14);  176us
 delay(60);     682us
 delay(5);      77us
 delay(2);      44us
 delay(25);     297us
 */
/**/
#define NOP() asm("nop");

void delay(unsigned int t)
{
    while(t--);
}
/*
void delay_ms(unsigned int t)
{
    while(t--)
    {
        NOP();
    }
}
*/
void delay_n(unsigned char n)
{
    while(n--)
    {
        delay(65535);
    }
}


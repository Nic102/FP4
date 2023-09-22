#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
using namespace std;

char KT = false;

void hexInBin(string a,bitset<32> &b) 
{
    string c;
    for(auto &elem : a)
        c.append([](char s)->string
        {
                switch (s)
                {
                    case '0': return "0000";
                    case '1': return "0001";
                    case '2': return "0010";
                    case '3': return "0011";
                    case '4': return "0100";
                    case '5': return "0101";
                    case '6': return "0110";
                    case '7': return "0111";
                    case '8': return "1000";
                    case '9': return "1001";
                    case 'A': return "1010";
                    case 'B': return "1011";
                    case 'C': return "1100";
                    case 'D': return "1101";
                    case 'E': return "1110";
                    case 'F': return "1111";
                    case 'a': return "1010";
                    case 'b': return "1011";
                    case 'c': return "1100";
                    case 'd': return "1101";
                    case 'e': return "1110";
                    case 'f': return "1111";
                }
                return "";
        }(elem));
    for (int i = 0; i < 32; i++) 
        if (c[i]=='1')
            b[31 - i] = 1;
        else if(c[i]=='0')
            b[31 - i] = 0;

}


void razdelenie(bitset<32> in_ch, bitset<1>& znak, bitset<8>& exp, bitset<23>& mantisa)
{
    znak[0] = in_ch[31];
    for (int i = 0; i < 31; i++)
        if (i < 23)  mantisa[i] = in_ch[i]; 
        else  exp[i - 23] = in_ch[i]; 
}


void BinInDec8(bitset<8> n, int& step)
{
    for (int i = 0; i < 8; i++)
        if (n[i] == 1)
            step += pow(2, i);
}


void Okruglenie(bitset<48>& vrem_1, int step)
{
    int i = 23;
    while (i < size(vrem_1))
        if (vrem_1[i] == 1)
        {
            if (i < (size(vrem_1) - 1))
            {
                vrem_1[i] = 0;
                i++;
            }
            else if (i == (size(vrem_1) - 1) and vrem_1[i] == 1)
            {
                vrem_1[i] = 0;
                vrem_1[23] = 1;
                step += 1;
                break;
            }
        }
        else if (vrem_1[i] == 0)
        {
            vrem_1[i] = 1;
            break;
        }
}


void Summator(bitset<32> one, bitset<32> two, bitset<32>& otv)
{
    setlocale(LC_ALL, "Russian");
    bitset<1> znak1, znak2;
    bitset<8> exp1, exp2;
    bitset<23> mantisa1, mantisa2;
    bitset<48> vrem1, vrem2, vrem;
    float num_in_fun1, num_in_fun2;
    char maximum = false, odin_exp = false,vivod_summ=false;
    int step = 0, step1 = 0, step2 = 0, i;

    razdelenie(one, znak1, exp1, mantisa1);
    razdelenie(two, znak2, exp2, mantisa2);
    BinInDec8(exp1, step1);
    BinInDec8(exp2, step2);

    if (znak1 != znak2 and exp1 == exp2 and mantisa1 == mantisa2)
        return;
    else if (znak1 != znak2 and exp1 == exp2 and mantisa1 != mantisa2)
        odin_exp = true;


    if ((znak1[0] == 1 or znak2[0] == 1) and (znak1 != znak2))
    {
        union
        {
            unsigned long long int input;
            float output;
        }data;
        data.input = one.to_ulong();
        num_in_fun1 = data.output;
        data.input = two.to_ulong();
        num_in_fun2 = data.output;

        if (abs(num_in_fun1) > abs(num_in_fun2))
            maximum = false;
        else if (abs(num_in_fun1) < abs(num_in_fun2))
            maximum = true;
    }

    for (int i = 0; i < size(vrem1); i++)
        if (i < (size(vrem1) - 2) / 2)
        {
            vrem1[i + (size(vrem1) - 2) / 2] = mantisa1[i];
            vrem2[i + (size(vrem1) - 2) / 2] = mantisa2[i];
        }

    vrem1[size(vrem1) - 2] = 1;
    vrem2[size(vrem2) - 2] = 1;
    if (step1 == step2)
        step = step1;
    else if (step1 > step2)
    {
        step = step1 - step2;
        vrem2 >>= step;
        step = step1;
    }
    else if (step1 < step2)
    {
        step = step2 - step1;      
        vrem1 = vrem1 >> step;
        step = step2;
    }
    if (KT == true) 
    {
        cout << "1) Разница exp ----------- " << abs(step2-step1) << endl;
        cout << "2) Денормализация -------- ";
        for (int i = size(vrem2) - 1; i >= 23; i--)
            cout << vrem1[i];
        cout << endl;
    }
        

    if ((znak1[0] == 1 or znak2[0] == 1) and (znak1 != znak2))
    {
        if (maximum == false)
            vrem2 = ~vrem2;
        else if (maximum == true)
            vrem1 = ~vrem1;
    }


    while (vrem2 != 0)
    {
        vrem = vrem1 & vrem2;
        vrem1 ^= vrem2;
        vrem2 = vrem << 1;
    }
    if (KT == true)
    {
        cout << "3) Cложение мантис ------- ";
        for (int i = size(vrem1) - 1; i >= 0; i--)
        {
            if (i == 22)
                cout << " ";
            cout << vrem1[i];
        };
        cout << endl;
        vivod_summ = true;
    }
    

    if ((znak1[0] == 1 or znak2[0] == 1) and (znak1 != znak2))
    {
        i = 0;
        while (i < size(vrem1))
            if (vrem1[i] == 1)
            {
                if (i < size(vrem1) - 1)
                {
                    vrem1[i] = 0;
                    i++;
                }
                else if (i == (size(vrem1) - 1) and vrem1[i] == 1)//?????????????? проверить
                {
                    vrem1[i] = 0;
                    vrem1[(size(vrem1) - 2) / 2] = 1;
                    break;
                }
            }
            else if (vrem1[i] == 0)
            {
                vrem1[i] = 1;
                break;
            }

        if (vivod_summ!=true)
        {
            cout << "3) Cложение мантис ------- ";
            for (int i = size(vrem1) - 1; i >= 0; i--)
            {
                if (i == 22)
                    cout << " ";
                cout << vrem1[i];
            }
            cout << endl;
        }

        while (vrem1[size(vrem1) - 1] == 0 and vrem1[size(vrem1) - 2] == 0)// здесть изменил для (y h 01800000 h 81ffff0f)
        {//
            step -= 1;//
            if (step > 0)
                vrem1 <<= 1;//
            else
                break;
        }//
    }

    if (vrem1[size(vrem1) - 1] != 0)
    {
        vrem1 >>= 1;
        step += 1;
        
    }

    if (KT == true)
    {
        cout << "4) Нормализация ---------- ";
        for (int i = size(vrem1) - 1; i >= 0; i--)
        {
            if (i == 22)
                cout << " ";
            cout << vrem1[i];
        }
        cout << endl;
    }

    if (vrem1[(size(vrem1) - 2) / 2] == 1 and vrem1[((size(vrem1) - 2) / 2) - 1] == 1)
    {
        Okruglenie(vrem1, step);

        if (vrem1[size(vrem1) - 1] != 0)
        {
            vrem1 >>= 1;
            step += 1;
        }

    }
    else if (vrem1[(size(vrem1) - 2) / 2] == 0 and vrem1[((size(vrem1) - 2) / 2) - 1] == 1)
    {
        for (int i = 0; i < ((size(vrem1) - 2) / 2) - 1; i++)
            if (vrem1[i] == 1)
            {
                Okruglenie(vrem1, step);
                break;
            }

        if (vrem1[size(vrem1) - 1] != 0)
        {
            vrem1 >>= 1;
            step += 1;
        }
    }

    if (KT == true)
    {
        cout << "5) Округление ------------ ";
        for (int i = size(vrem1) - 1; i >= 23; i--)
            {cout << vrem1[i];}
        cout << endl;
    }

    for (int i = ((size(vrem1) / 2) + 1); i < size(vrem1); i++)
        otv[i - ((size(vrem1) / 2) + 1)] = vrem1[i - 2];

    if ((znak1[0] == 1 and znak2[0] == 1) or (znak1[0] == 1 and maximum == false) or (znak2[0] == 1 and maximum == true))
        otv[size(otv) - 1] = 1;

    bitset<8>bin_exp(step);
    exp1 = bin_exp;

    for (int i = 0; i < 8; i++)
        otv[((size(vrem1) - 2) / 2) + i] = exp1[i];
}


int main()
{
    setlocale(LC_ALL, "Russian");
    bitset<32> in_ch, in_ch2, test, test1, test2, otv;
    float num_1, num_2, res, vvod1, vvod2;
    int errors = 0, i, q = 0, q1 = 0, q2 = 0, q3 = 0, q4 = 0, exp_len = 0, mant_len = 0;
    string nestandartniy_otv1, nestandartniy_otv2, hex1, hex2;
    char pref1, pref2, KT_in;

    cout << endl << "                                СУММАТОР\n";
    cout << "Нужны ли Контрольные точки? y-да, n-нет" << endl;
    cin >> KT_in;
    if (KT_in == 'y') 
        KT = true;
    else if (KT_in=='n')
    {
        KT = false;
    }
    else 
    {
        cout << "Не правильный ввод Контрольной точки" << endl;
    }
    cout << "Введите префиксы 1го и 2го числа и сами числа через пробел.\n";
    cout << "Префиксы --- (d -> dec,h -> hex,b -> bin)\n\n";

    cin >> pref1;
    if (pref1 == 'b')
        cin >> in_ch;
    else if (pref1 == 'd')
    {
        cin >> vvod1;
        union
        {
            float input;
            int output;
        }aa1;
        aa1.input = vvod1;
        bitset<32>z1(aa1.output);
        in_ch = z1;
    }
    else if (pref1 == 'h')
    {
        cin >> hex1;
        hexInBin(hex1,in_ch);
    }

    cin >> pref2;
    if (pref2 == 'b')
        cin >> in_ch2;
    else if (pref2 == 'd')
    {
        cin >> vvod2;

        union
        {
            float input;
            int output;
        }aa2;
        aa2.input = vvod2;
        bitset<32>z2(aa2.output);
        in_ch2 = z2;
    }
    else if (pref2 == 'h')
    {

        cin >> hex2;
        hexInBin(hex2, in_ch2);
    }


    cout << "Первое число Bin: ";
    for (int i = size(in_ch)-1; i >= 0; i--) 
    {
        cout << in_ch[i];
        if (i == 23 or i==size(in_ch)-1) 
            cout << " ";
    }
    cout << "      Hex: " << hex << in_ch.to_ulong() << "  Dec: " << dec << in_ch.to_ulong() << endl;
    cout << "Второе число Bin: ";
    for (int i = size(in_ch2) - 1; i >= 0; i--)
    {
        cout << in_ch2[i];
        if (i == 23 or i == size(in_ch2) - 1)
            cout << " ";
    }
    cout << "      Hex: " << hex << in_ch2.to_ulong() << "  Dec: " << dec << in_ch2.to_ulong() << endl;

    q1, q2, q3, q4 = 0;
    for (int i = 0; i < 32; i++)
    {
        if (i < 23 and in_ch[i] == 1)
            q1++;
        if (i >= 23 and i < 32 and in_ch[i] == 1)
            q2++;
        if (i < 23 and in_ch2[i] == 1)
            q3++;
        if (i >= 23 and i < 32 and in_ch2[i] == 1)
            q4++;
    }    

    if (q2 == 8)
        if (q1 > 0)
            nestandartniy_otv1 = "Nan";
        else
            nestandartniy_otv1 = "inf";
    else if (q1 == 0 and q2 == 0)
        nestandartniy_otv1 = "0";
    else
        nestandartniy_otv1 = "";

    if (q4 == 8)
        if (q3 > 0)
            nestandartniy_otv2 = "Nan";
        else
            nestandartniy_otv2 = "inf";
    else if (q3 == 0 and q4 == 0)
        nestandartniy_otv1 = "0";
    else
        nestandartniy_otv1 = "";

    if (nestandartniy_otv1 == "Nan" or nestandartniy_otv2 == "Nan") { cout << endl << "Nan" << endl; }
    else if (nestandartniy_otv1 == "inf" and nestandartniy_otv2 == "0") { cout << endl << "inf" << endl; }
    else if (nestandartniy_otv1 == "0" and nestandartniy_otv2 == "inf") { cout << endl << "inf" << endl; }
    else if (nestandartniy_otv1 == "0" and nestandartniy_otv2 == "0") { cout << endl << "0" << endl; }
    else if (nestandartniy_otv1 == "0" and nestandartniy_otv2 == "") { cout << endl << in_ch2 << endl; }
    else if (nestandartniy_otv1 == "" and nestandartniy_otv2 == "0") { cout << endl << in_ch << endl; }
    else if ((q2 == 0 and q1 != 0) or (q4 == 0 and q3 != 0)) { cout << endl << "Вы ввели денормализованное число" << endl; }
    else
    {

        Summator(in_ch, in_ch2, otv);

        union
        {
            unsigned long long int input;
            float output;
        }data;
        data.input = in_ch.to_ulong();
        num_1 = data.output;
        data.input = in_ch2.to_ulong();
        num_2 = data.output;
        res = num_1 + num_2;

        union
        {
            float input;
            int output;
        }aaa;
        aaa.input = res;
        bitset<32>proverka(aaa.output);
        cout << endl;
        cout << "Результат Bin:          ";
        for (int i = size(otv) - 1; i >= 0; i--)
        {
            cout << otv[i];
            if (i == 23 or i == size(otv) - 1)
                cout << " ";
        }
        cout << "      Hex: " << hex << otv.to_ulong() << "  Dec: " << dec << otv.to_ulong() << endl;

        errors = 0;
        for (int i = 0; i < size(otv); i++)
            if (proverka[i] != otv[i])
                errors++;

        if (errors != 0)
        {
            cout << "ОШИБКА"<<endl;
            cout <<"Правильный результат -- ";
            for (int i = size(proverka) - 1; i >= 0; i--)
            {
                cout << proverka[i];
                if (i == 23 or i == size(proverka) - 1)
                    cout << " ";
            }
            cout << endl;
        }
    }
    return main(); }

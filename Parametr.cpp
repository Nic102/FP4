#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
using namespace std;


void razdelenie(bitset<32> in_ch, bitset<1>& znak, bitset<8>& exp, bitset<23>& mantisa)
{
    znak[0] = in_ch[31];
    for (int i = 0; i < 31; i++)
        if (i < 23) { mantisa[i] = in_ch[i]; }
        else { exp[i - 23] = in_ch[i]; }
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
    char maximum = false, odin_exp = false;
    int step = 0, step1 = 0, step2 = 0, i;

    razdelenie(one, znak1, exp1, mantisa1);
    razdelenie(two, znak2, exp2, mantisa2);
    BinInDec8(exp1, step1);
    BinInDec8(exp2, step2);

    if (znak1 != znak2 and exp1 == exp2 and mantisa1 == mantisa2)
    {
        return;
    }
    else if (znak1 != znak2 and exp1 == exp2 and mantisa1 != mantisa2)
    {
        odin_exp = true;
    }

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
        {
            maximum = false;
        }
        else if (abs(num_in_fun1) < abs(num_in_fun2))
        {
            maximum = true;
        }
    }

    for (int i = 0; i < size(vrem1); i++)//перезапись мантисы размером 23 бита в буферную переменную размером 48 бит (Возможно можно сократить!!!!)
    {
        if (i < (size(vrem1) - 2) / 2)
        {
            vrem1[i + (size(vrem1) - 2) / 2] = mantisa1[i];
            vrem2[i + (size(vrem1) - 2) / 2] = mantisa2[i];
        }
    }

    //cout << endl << "Мантиса 1 ------------ " << vrem1 << endl;
    //cout << "Мантиса 2 ------------ " << vrem2 << endl << endl;

    vrem1[size(vrem1) - 2] = 1;//К мантисе добавили 1 бит с лева
    vrem2[size(vrem2) - 2] = 1;

    //cout << "+1 для 1 мнимая ------ " << vrem1 << endl;
    //cout << "+1 для 2 мнимая ------ " << vrem2 << endl << endl;

    if (step1 == step2)
    {
        step = step1;
    }
    else if (step1 > step2)//разница мантис
    {
        step = step1 - step2;
        vrem2 >>= step;
        step = step1;
        //cout << "нормализовали 2 ------ " << vrem2 << endl << endl;
    }
    else if (step1 < step2)
    {
        step = step2 - step1;
        vrem1 = vrem1 >> step;
        step = step2;
        //cout << "нормализовали 1 ------ " << vrem1 << endl << endl;
    }

    if ((znak1[0] == 1 or znak2[0] == 1) and (znak1 != znak2))
    {
        if (maximum == false)//Инвертирую ну мантису которая меньше
        {
            vrem2 = ~vrem2;
            //cout << "инвертировали 2 ------ " << vrem2 << endl << endl;
        }
        else if (maximum == true)
        {
            vrem1 = ~vrem1;
            //cout << "инвертировали 1 ------ " << vrem1 << endl << endl;
        }
    }

    //cout << "до сложения 1 -------- " << vrem1 << endl;
    //cout << "до сложения 2 -------- " << vrem2 << endl;

    while (vrem2 != 0)
    {
        vrem = vrem1 & vrem2;
        vrem1 ^= vrem2;
        vrem2 = vrem << 1;
    }

    //cout << endl << "Сумма двух мантис ---- " << vrem1 << endl << endl;

    if ((znak1[0] == 1 or znak2[0] == 1) and (znak1 != znak2))
    {
        i = 0;
        while (i < size(vrem1))//-------------добавить 1 бит
            if (vrem1[i] == 1)
            {
                if (i < size(vrem1) - 1)
                {
                    vrem1[i] = 0;
                    i++;
                }
                else if (i == (size(vrem1) - 1) and vrem1[i] == 1)
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

        //cout << "+1 бит т.к отриц ----- " << vrem1 << endl << endl;

        if ((exp1 != exp2))
        {
            while (vrem1[size(vrem1) - 1] == 0 and vrem1[size(vrem1) - 2] == 0)//если 00 в начале 
            {
                vrem1 <<= 1;
                step -= 1;
            }
        }
        //cout << "нормализация --------- " << vrem1 << endl << endl;

    }

    if (vrem1[size(vrem1) - 1] != 0)
    {
        vrem1 >>= 1;
        step += 1;
    }

    if (vrem1[(size(vrem1) - 2) / 2] == 1 and vrem1[((size(vrem1) - 2) / 2) - 1] == 1)//Округление если 1 в конце мантисы и хвост начинается с 1
    {
        Okruglenie(vrem1, step);

        if (vrem1[size(vrem1) - 1] != 0)
        {
            vrem1 >>= 1;
            step += 1;
        }

        //cout << "       okruglenie ---- " << vrem1 << endl << endl;
    }
    else if (vrem1[(size(vrem1) - 2) / 2] == 0 and vrem1[((size(vrem1) - 2) / 2) - 1] == 1)//Округление если 0 в конце мантисы и хвост начинается с 1 и есть хотя бы одна единица в хвосте кроме первой
    {
        for (int i = 0; i < ((size(vrem1) - 2) / 2) - 1; i++)
        {
            if (vrem1[i] == 1)
            {
                Okruglenie(vrem1, step);
                break;
            }
        }

        if (vrem1[size(vrem1) - 1] != 0)
        {
            vrem1 >>= 1;
            step += 1;
        }

        //cout << "       okruglenie ---- " << vrem1 << endl << endl;
    }

    for (int i = ((size(vrem1) / 2) + 1); i < size(vrem1); i++)//перезапись буферной переменной мантисы в переменную ответа
    {
        otv[i - ((size(vrem1) / 2) + 1)] = vrem1[i - 2];
    }

    if ((znak1[0] == 1 and znak2[0] == 1) or (znak1[0] == 1 and maximum == false) or (znak2[0] == 1 and maximum == true))
    {
        otv[size(otv) - 1] = 1;
    }

    bitset<8>bin_exp(step);
    exp1 = bin_exp;

    for (int i = 0; i < 8; i++)
    {
        otv[((size(vrem1) - 2) / 2) + i] = exp1[i];
    }
}


int main()
{
    setlocale(LC_ALL, "Russian");
    bitset<32> in_ch, in_ch2, test, test1, test2, otv;
    float num_1, num_2, res, vvod1, vvod2;
    int errors = 0, i, q = 0, q1 = 0, q2 = 0, q3 = 0, q4 = 0, exp_len = 0, mant_len = 0;
    string nestandartniy_otv1, nestandartniy_otv2, hex1, hex2;
    char pref1, pref2;

    //in_ch =  0b10000000100000000000000000000001;
    //in_ch2 = 0b00010110011111111111111111111111;
    //          ^      ^
    //for (int i = 0; i < 500; i++)
    //{
        //test1 = 0b00000000000000000000000000000001;
        //test2 = 0b00000000000000000000000000000001;
        //         ^      ^
    /*cout << endl << "                                СУММАТОР\n";
    cout << "Введите размер мантисы и экспаненты, экспанента не более 8, мантиса не более 23 " << endl;
    cout << "Пример : 8 23"<<endl;
    cin >> exp_len;
    cin >> mant_len;*/

    cout << "Введите префиксы 1го и 2го числа и сами числа через пробел.\n";
    cout << "Префиксы --- (d -> dec,h -> hex,b -> bin)\n\n";
    cout << "Внимание! Если ввод состоит из Bin, то вводить только нормализованные, т.е не меньше 0 00000001 00000000000000000000000" << endl;
    cout << "Примеры ввода: b 00000000100000000000000000000001 b 00010110011111111111111111111111" << endl;
    cout << "Примеры ввода: d 123.42 d -0.12311" << endl;
    cout << "Примеры ввода: h 42f69802 d 123 " << endl << endl;
    //pref1 = 'b';
    //pref2 = 'b';
    /*while (test1 != 0)
    {
        test = in_ch & test1;
        in_ch ^= test1;
        test1 = test << 1;

    }
    while (test2 != 0)
    {
        test = in_ch2 & test2;
        in_ch2 ^= test2;
        test2 = test << 1;

    }*/
    cin >> pref1;
    if (pref1 == 'b')
    {
        cin >> in_ch;
    }
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
        vvod1 = stoi(hex1, 0, 16);

        union
        {
            float input;
            int output;
        }aa1;
        aa1.input = vvod1;
        bitset<32>z1(aa1.output);
        in_ch = z1;
    }

    cin >> pref2;
    if (pref2 == 'b')
    {
        cin >> in_ch2;
    }
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
        vvod2 = stoi(hex2, 0, 16);
        union
        {
            float input;
            int output;
        }aa2;
        aa2.input = vvod2;
        bitset<32>z2(aa2.output);
        in_ch2 = z2;
    }


    cout << endl << "Первое число Bin: " << in_ch << "      Hex: " << hex << in_ch.to_ulong() << "  Dec: " << dec << in_ch.to_ulong() << endl;
    cout << "Второе число Bin: " << in_ch2 << "      Hex: " << hex << in_ch2.to_ulong() << "  Dec: " << dec << in_ch2.to_ulong() << endl;
    cout << "                   ^      ^";
    q1, q2, q3, q4 = 0;
    for (int i = 0; i < 32; i++)
    {
        if (i < 23 and in_ch[i] == 1)
        {
            q1++;
        }
        if (i >= 23 and i < 32 and in_ch[i] == 1)
        {
            q2++;
        }
        if (i < 23 and in_ch2[i] == 1)
        {
            q3++;
        }
        if (i >= 23 and i < 32 and in_ch2[i] == 1)
        {
            q4++;
        }
    }

    if (q2 == 8)
    {
        if (q1 > 0)
        {
            nestandartniy_otv1 = "Nan";
        }
        else
        {
            nestandartniy_otv1 = "inf";
        }
    }
    else if (q1 == 0 and q2 == 0)
    {
        nestandartniy_otv1 = "0";
    }
    else
    {
        nestandartniy_otv1 = "";
    }

    if (q4 == 8)
    {
        if (q3 > 0)
        {
            nestandartniy_otv2 = "Nan";
        }
        else
        {
            nestandartniy_otv2 = "inf";
        }
    }
    else if (q3 == 0 and q4 == 0)
    {
        nestandartniy_otv1 = "0";
    }
    else
    {
        nestandartniy_otv1 = "";
    }


    if (nestandartniy_otv1 == "Nan" or nestandartniy_otv2 == "Nan") { cout << endl << "Nan" << endl; }
    else if (nestandartniy_otv1 == "inf" and nestandartniy_otv2 == "0") { cout << endl << "inf" << endl; }
    else if (nestandartniy_otv1 == "0" and nestandartniy_otv2 == "inf") { cout << endl << "inf" << endl; }
    else if (nestandartniy_otv1 == "0" and nestandartniy_otv2 == "0") { cout << endl << "0" << endl; }
    else if (nestandartniy_otv1 == "0" and nestandartniy_otv2 == "") { cout << endl << in_ch2 << endl; }
    else if (nestandartniy_otv1 == "" and nestandartniy_otv2 == "0") { cout << endl << in_ch << endl; }
    //else if (abs(vvod1) == abs(vvod2) and (in_ch[31]==1 or in_ch2[31]==1) and (in_ch[31]!=in_ch2[31]))
    //{
    //    cout << "0" << endl;
    //}
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

        //cout << endl << "Надо         " << "Bin  " << proverka << "      Hex " << hex << proverka.to_ulong() << "   Dec " << dec << proverka.to_ulong() << endl;
        cout << endl << "Результат    " << "Bin  " << otv << "      Hex " << hex << otv.to_ulong() << "   Dec " << dec << otv.to_ulong() << endl;
        cout << "                   ^      ^" << endl;
        errors = 0;
        for (int i = 0; i < size(otv); i++)
        {
            if (proverka[i] != otv[i])
            {
                errors++;
            }
        }
    }
    if (errors != 0)
    {
        cout << "Количество не совпадений --------------------------------------- " << errors << endl;
        cout << "АХТУНГ,АЛЯРМ, ОШИБКА"; //break;
    }
    //}
}

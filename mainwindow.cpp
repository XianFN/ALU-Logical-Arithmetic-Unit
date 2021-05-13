#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std; //Permite usar string


float n1;
float n2;

vector<unsigned int> esBinario;

struct IEE754{
    unsigned int signo;
    unsigned int exponente[8];
    unsigned int mantisa[23];
} sN1, sN2;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * Trasformamos un entero a binario
 * @brief decimalABinario
 * @param numero
 * @return
 */
vector<unsigned int> decimalABinario(int numero){

    vector<unsigned int> binario;
    vector<unsigned int> ret;

    while(numero != 1){
        unsigned int binarioEntero = numero % 2; //Calculamos el binario a guardar
        numero = numero /2; //Actualizamos nuemro
        binario.push_back(binarioEntero);//Guardamos el binario
    }

    binario.push_back(numero); //Guardamos el ultimo numero
    //posComa = binario.size(); //Guardamos la posicion de la coma para el binarion decimal
    for(int i = binario.size() -1; i >=0; i--){ //Giramos el array
        ret.push_back(binario[i]);

    }
    return ret;
}


/**
 * Calculamos el signo
 * @brief calcularSigno
 * @param n
 * @return
 */
unsigned int calcularSigno(float n){

    unsigned int ret;

    if(n < 0){
        ret = 1; //negativo
    }else{
        ret = 0;
    }

    return ret;
}


/**
 * Calculamos el exponente del numero
 * @brief calcularExponenete
 * @param n
 * @return
 */
vector<unsigned int> calcularExponenete(float n){ //TODO falla al calcular numeros negativos, revisar
    vector<unsigned int> binario;
    esBinario.clear();

    int entera = static_cast<int>(n);
    float decimal= n - entera;
    int numero = entera;
    int posComa = -1;


    if(decimal == 0){ //Cuando el numero n es entero
        cout<<"ENTERO"<<endl;
        esBinario = decimalABinario(numero);
        posComa = esBinario.size();

    }else{ //Cuando el numero es decimal


        esBinario = decimalABinario(numero);
        posComa = esBinario.size();

        float pDecimal = decimal;
        vector<float> pDecimalGuardada;
        bool flag = true;



        for(int i = 0; i < 24; i++){ //Bucle con el que calculamos la parte decimal de un numero

            float resultado = pDecimal *2.0f; //Guardamos el resultado de la multiplicaicon
            unsigned int partEnteraResultado = static_cast<unsigned int>(resultado); //Parte entera, que es el Nº binario
            float partDecimaResultado = resultado - partEnteraResultado; //parte decimal, para actualzar y seguir multiplicando
            partDecimaResultado = round(partDecimaResultado*100)/100; //redondeamos a dos decimales
            esBinario.push_back(partEnteraResultado); //Guardamos el return
            pDecimal = partDecimaResultado;

        }

    }

    cout<<"Imprimir ret"<<endl;
    for(int i = 0; i < esBinario.size(); i++){
        cout<<esBinario[i];
    }
    cout<<endl;

    cout<<"Posicion de la coma: "<<posComa<<endl;

    return decimalABinario(127+posComa-1);
}


vector<unsigned int> calcularMaintisa(){

    vector<unsigned int> ret;


    for(int i = 1; i <= 23; i++){

        if(i >= esBinario.size()){
            ret.push_back(0);
        }else{
            ret.push_back(esBinario[i]);
        }

    }
        cout<<"MANTISA"<<endl;
    for(int i = 0; i < ret.size(); i++){

        cout<<ret[i];
    }
    cout<<endl;


    return ret;

}

void MainWindow::on_pushButton_clicked()
{
    string operacion;
    string mostrarIee754N1;
    string mostrarIee754N2;

    ui->n1Iee->clear();
    ui->n2Iee->clear();

    if(ui->bSuma->isChecked()){//Miramos que operacion tenemos que realizar
        operacion = "sum";
    }else if(ui->bMultiplicacion->isChecked()){
        operacion = "mult";
    }else if(ui->bDivision->isChecked()){
        operacion = "divs";
    }

    n1 = ui->n1->text().toFloat(); //Obtenemos los numeros a calcular
    n2 = ui->n2->text().toFloat();

    cout<<n1<<endl;

    sN1.signo = calcularSigno(n1);//Calculamos el signo

    cout<<sN1.signo<<endl;


    vector<unsigned int> expBuffer = calcularExponenete(n1); //Retornamos el exponenete de la funcion
    copy(expBuffer.begin(), expBuffer.end(), sN1.exponente); //Copiamos el vector al array


    vector<unsigned int> manBuffer = calcularMaintisa(); //Retornamos el exponenete de la funcion
    copy(manBuffer.begin(), manBuffer.end(), sN1.mantisa); //Copiamos el vector al array




    sN2.signo = calcularSigno(n2);

    vector<unsigned int> expBuffer2 = calcularExponenete(n2);
    copy(expBuffer2.begin(), expBuffer2.end(), sN2.exponente);

    vector<unsigned int> manBuffer2 = calcularMaintisa();
    copy(manBuffer2.begin(), manBuffer2.end(), sN2.mantisa);








    mostrarIee754N1.append(to_string(sN1.signo));//Añadimos al string para mostrar
    mostrarIee754N1.append("  ");

    for(int i = 0; i < 8;i++){
        mostrarIee754N1.append(to_string(sN1.exponente[i]));//Añadimos al string para mostrar
    }
    mostrarIee754N1.append("  ");

    for(int i = 0; i < 23;i++){
        mostrarIee754N1.append(to_string(sN1.mantisa[i]));//Añadimos al string para mostrar
    }

    ui->n1Iee->setText(QString::fromLocal8Bit(mostrarIee754N1.c_str()));//Imprimimos el resultado



    mostrarIee754N2.append(to_string(sN2.signo));//Añadimos al string para mostrar
    mostrarIee754N2.append("  ");

    for(int i = 0; i < 8;i++){
        mostrarIee754N2.append(to_string(sN2.exponente[i]));//Añadimos al string para mostrar
    }

    mostrarIee754N2.append("  ");

    for(int i = 0; i < 23;i++){
        mostrarIee754N2.append(to_string(sN2.mantisa[i]));//Añadimos al string para mostrar
    }

    ui->n2Iee->setText(QString::fromLocal8Bit(mostrarIee754N2.c_str()));//Imprimimos el resultado




}






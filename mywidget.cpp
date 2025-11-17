#include "mywidget.h"
#include "ui_mywidget.h"

#include<QFile>
#include<QDebug>
#include<QFileDialog>
#include<QTextStream>
#include<QApplication>

QString mFilename = "C:/Users/user/Desktop/EX/myfile.txt";

void Write(QString Filename,QString str)
{
    QFile mFile(Filename);
    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug()<<"could not open file for write";
        return;
    }
    QTextStream out(&mFile);
    out<<str;
    mFile.flush();
    mFile.close();
}
QString Read(QString Filename)
{
    QFile mFile(Filename);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug()<<"could not open file for read";
        return "";
    }
    QTextStream in(&mFile);
    QString text = in.readAll();
    mFile.close();
    return text;
}

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    QStringList ColTotle;
    ui->tableWidget->setColumnCount(4);
    ColTotle<<QStringLiteral("學號")<<QStringLiteral("班級")<<QStringLiteral("姓名")<<QStringLiteral("電話");
    ui->tableWidget->setHorizontalHeaderLabels(ColTotle);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_pushButton_2_clicked()
{
    QTableWidgetItem *inputRow1,*inputRow2,*inputRow3,*inputRow4;

    inputRow1 = new QTableWidgetItem(QString(ui->lineEdit_2->text()));
    inputRow2 = new QTableWidgetItem(QString(ui->lineEdit_3->text()));
    inputRow3 = new QTableWidgetItem(QString(ui->lineEdit_4->text()));
    inputRow4 = new QTableWidgetItem(QString(ui->lineEdit->text()));

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,inputRow1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,inputRow2);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,inputRow3);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,inputRow4);

    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
}


void MyWidget::on_pushButton_clicked()
{
    QString saveFile="";
    mFilename=QFileDialog::getSaveFileName(this,"匯出存檔",".");
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        for(int j=0;j<ui->tableWidget->columnCount();j++){
            saveFile+=ui->tableWidget->item(i,j)->text()+",";
        }
        saveFile+="\n";
    }
    Write(mFilename,saveFile);
}
void MyWidget::on_pushButton_3_clicked()
{
    mFilename=QFileDialog::getOpenFileName(this,"匯入存檔",".");
    QString fileContent = Read(mFilename);
    if(fileContent.isEmpty()){
        qDebug()<<"File is empty or could not be read";
        return;
    }

    // Clear existing table data
    ui->tableWidget->setRowCount(0);

    // Split by lines
    QStringList lines = fileContent.split("\n", Qt::SkipEmptyParts);

    for(int i=0; i<lines.size(); i++){
        QString line = lines[i];
        QStringList fields = line.split(",");

        if(fields.size() >= 4){
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());

            for(int j=0; j<4 && j<fields.size(); j++){
                QTableWidgetItem *item = new QTableWidgetItem(fields[j]);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, j, item);
            }
        }
    }
}


void MyWidget::on_pushButton_4_clicked()
{
    // Save data first
    on_pushButton_clicked();
    // Then close the application
    close();
    QString text = Read(mFilename);
    if(text.isEmpty()){
        qDebug()<<"No data to import or file is empty";
        return;
    }

    // Clear existing table data
    ui->tableWidget->setRowCount(0);

    // Split by lines
    QStringList lines = text.split("\n", Qt::SkipEmptyParts);

    for(const QString &line : lines){
        // Split by comma
        QStringList fields = line.split(",");

        if(fields.size() == 4){
            // Add a new row
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);

            // Set the data for each column
            for(int i = 0; i < 4; i++){
                QTableWidgetItem *item = new QTableWidgetItem(fields[i]);
                ui->tableWidget->setItem(row, i, item);
            }
        }
    }
}


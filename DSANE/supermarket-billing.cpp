#include<iostream>
#include<fstream>

using namespace std;

class shopping
{

private:
    int pcode;
    float price;
    float dis;
    string pname;

public:
    void menu();
    void administrator();
    void buyer();
    void add();
    void edit();
    void rem();
    void list();
    void receipt();
};

void shopping:: menu()
{
    m:
    int choice;
    string email;
    string password;

    cout<<"\t\t\t\t\t**************************************\n";
    cout<<"\t\t\t\t\t                                      \n";
    cout<<"\t\t\t\t\t      Supermarket Main Menu           \n";
    cout<<"\t\t\t\t\t                                      \n";
    cout<<"\t\t\t\t\t**************************************\n";
    cout<<"\t\t\t\t\t| 1) Administrator  |\n";
    cout<<"\t\t\t\t\t|                   |\n";
    cout<<"\t\t\t\t\t| 2) Buyer          |\n";
    cout<<"\t\t\t\t\t|                   |\n";
    cout<<"\t\t\t\t\t| 3) Exit           |\n";
    cout<<"\t\t\t\t\t|                   |\n";
    cout<<"\n\t\t\t\t Please select : ";
    cin>>choice;

    switch(choice)
    {

        case 1:
            cout<<"\t\t\t PLEASE LOGIN \n \n";
            cout<<"\t\t\t Enter Email : ";
            cin>>email;
            cout<<"\n\t\t\t Password : ";
            cin>>password;

            if(email == "nibakure@gmail.com" && password == "nibakure")
            {

                administrator();
            }
            else{
                cout<<"\n\t\t\tInvalid email/password!!\n\n";
            }
            break;

      case 2:
        {
               buyer();
        }

       case 3:
            {
                exit(0);

            }
      default:
        {
            cout<<"Please select from the given options";
        }

    }
    goto m;
   }

  void shopping::administrator()
  {
      m:
      int choice;
      cout<<"\n\n\n\t\t\t\t Admistrator menu";
      cout<<"\n\t\t\t\t|____1) Add the product____|";
      cout<<"\n\t\t\t\t|                          |";
      cout<<"\n\t\t\t\t|____2) Modify the product_|";
      cout<<"\n\t\t\t\t|                          |";
      cout<<"\n\t\t\t\t|____3)Delete the product__|";
      cout<<"\n\t\t\t\t|                          |";
      cout<<"\n\t\t\t\t|____4) Back to main menu__|";

      cout<<"\n\n\t\t\t Please enter the choice : ";
      cin>>choice;

      switch(choice)
      {
     case 1:
        add();
        break;

     case 2:
         edit();
         break;

     case 3:
          rem();
           break;

     case 4:
        menu();
        break;

     default:
        cout<<"Invalid choice";
      }
      goto m;
    }

 void shopping:: buyer()
 {
     m:
     int choice;
     cout<<"\n\t\t\t  Buyer \n";
     cout<<"                 \n";
     cout<<"\t\t\t1) Buy product\n";
     cout<<"                \n";
     cout<<"\t\t\t2) Go back  \n";
     cout<<"\t\t\t Enter your choice: ";

     cin>>choice;

     switch(choice)
     {
         case 1:
             receipt();
             break;
         case 2:
            menu();
            break;

         default:
            cout<<"invalid choice:";
     }
     goto m;
 }

 void shopping::add()
 {
     m:
     fstream data;
     int c;
     int token =0;
     float p;
     float d;
     string n;

     cout<<"\n\n\t Add new product";
     cout<<"\n\n\t Product code of the product -> ";
     cin>>pcode;
     cout<<"\n\n\t Name of the product -> ";
     cin>>pname;
     cout<<"\n\n\t Price of the product -> ";
     cin>>price;
     cout<<"\n\n\t Discount on product -> ";
     cin>>dis;

     data.open("database.txt", ios::in); //ios::in will open the file in read mode;

     if(!data)
     {
         data.open("database.txt", ios::app| ios::out);// ios::app stands for append mode and ios::out stands for write mode;
         data<<" "<<pcode<<" "<<pname<<" "<<price<<" "<<dis<<"\n";
         data.close();
     }
     else
     {
         data>>c>>n>>p>>d;

         while(!data.eof())
         {
             if(c== pcode)
             {
                 token++;
             }
             data>>c>>n>>p>>d;
         }
         data.close();

     if(token==1)
     goto m;
      else {
         data.open("database.txt", ios::app| ios::out);// ios::app stands foir append mode and ios::out stands for write mode;
         data<<" "<<pcode<<" "<<pname<<" "<<price<<" "<<dis<<"\n";
         data.close();
      }
    }
      cout<<"\n\n\t\t Record inserted! ";
 }

 void shopping:: edit()
 {
     fstream data,data1;
     int pkey;
     int token=0;
     int c;
     float p;
     float d;
     string n;

     cout<<"\n\t\t\t Modify th record";
     cout<<"\n\t\t\t Product code : ";
     cin>>pkey;

     data.open("database.txt", ios::in);
     if(!data)
     {
         cout<<"\n\nFile doesn't exist";

     }
     else{
        data1.open("database1.txt",ios::app| ios::out);

        data>>pcode>>pname>>price>>dis;
        while(!data.eof())
        {
            if(pkey==pcode)
            {
                cout<<"\n\t\t Product new code: ";
                cin>>c;
                cout<<"\n\t\t Name of the product : ";
                cin>>n;
                cout<<"\n\t\t Price: ";
                cin>>p;
                cout<<"\n\t\t Discount : ";
                cin>>d;
                data1<<" "<<c<<" "<<n<<" "<<p<<" "<<d<<"\n";
                cout<<"\n\n\t\t Record edited \n";
                token++;
            }
            else
            {
                data1<<" "<<pcode<<" "<<pname<<" "<<price<<" "<<dis<<"\n";
            }
            data>>pcode>>pname>>price>>dis;
        }
        data.close();
        data1.close();

        remove("database.txt");
        rename("database1.txt", "database.txt");

        if(token==0)
        {
            cout<<"\n\n Record not found sorry!";
        }
     }
 }
void shopping::rem()
{
   fstream data,data1;
   int pkey;
   int token=0;
   cout<<"\n\n\t Delete product";
   cout<<"\n\n\t Product code : ";
   cin>>pkey;
   data.open("database.txt",ios::in);
   if(!data)
   {
       cout<<"File doesn't exist";
   }
   else
   {
       data1.open("database1.txt",ios::app|ios::out);
       data>>pcode>>pname>>price>>dis;
       while(!data.eof())
       {
           if(pcode==pkey)
           {
               cout<<"\n\n\t Product deleted successfully";
               token++;
           }
           else
           {

               data1<<" "<<pcode<<" "<<pname<<" "<<price<<" "<<dis<<"\n";
           }
           data>>pcode>>pname>>price>>dis;
       }
       data.close();
       data1.close();
       remove("database.txt");
       rename("database1.txt","database.txt");

       if(token==0)
       {
           cout<<"\n\n Record not found";
       }
   }
}
 void shopping::list()
 {
     fstream data;
     data.open("database.txt",ios::in);
     cout<<"\n\n\t\t___________________________________";
     cout<<"\n\t\tProNo\t\tName\t\tPrice\n";
     cout<<"\t\t___________________________________\n";
     data>>pcode>>pname>>price>>dis;
     while(!data.eof())
     {
         cout<<"\t\t"<<pcode<<"\t\t"<<pname<<"\t\t"<<price<<"\n";
         data>>pcode>>pname>>price>>dis;
     }
     data.close();

 }
 void shopping:: receipt()
 {
     fstream data;

     int arrc[100];
     int arrq[100];
     char choice;
     int c=0;
     float amount=0;
     float dis=0;
     float total=0;

     cout<<"\n\n\t\t\t RECEIPT";
     data.open("database.txt",ios::in);
     if(!data)
     {
         cout<<"\n\n Empty database"        ;
     }
     else
     {
         data.close();

         list();
         cout<<"\n\n\t\t_____________________________________";
         cout<<"\n\t\t|                                      |";
         cout<<"\n\t\t|       Please place the order         |";
         cout<<"\n\t\t|                                      |";
         cout<<"\n\t\t _______________________________________ ";

         do
         {
             m:
             cout<<"\n\n\t\t Enter product code : ";
             cin>>arrc[c];
             cout<<"\n\n\t\t Enter the product quantity : ";
             cin>>arrq[c];
             for(int i=0;i<c;i++)
             {
                 if(arrc[c]== arrc[i])
                 {
                     cout<<"\n\n\t Duplicate product code. Please try again!";
                     goto m;
                 }
             }
                 c++;
                 cout<<"\n\n\t Do you want to buy another product? if yes then press y else n : ";
                 cin>>choice;


            }
             while(choice == 'y');
             cout<<"\n***No of items available** "<<c;
             cout<<"\n\n\t\t\t________________RECEIPT_____________\n";
             cout<<"\n\t\tProduct No"<<"\t Product Name"<<"\t Product quantity"<<"\t price"<<"\t  Amount"<<"\t Amount with discount\n";

             for(int i=0;i<c;i++)
             {
                 data.open("database.txt",ios::in);
                 data>>pcode>>pname>>price>>dis;
                 while(!data.eof())
                 {
                     if(pcode==arrc[i])
                     {
                         amount= price*arrq[i];
                         dis= amount - (amount*dis/100);
                         total= total+dis;
                         cout<<"\n\t\t"<<pcode<<"\t\t "<<pname<<"\t\t  "<<arrq[i]<<"\t\t\t  "<<price<<"\t  "<<amount<<"\t\t  "<<dis;

                     }
                     data>>pcode>>pname>>price>>dis;
                 }

             }
              data.close();
         }
         cout<<"\n\n\t\t_________________________________________";
        cout<<"\n\t\t Total Amount : "<<total<<"\n";
     }
int main()
{
   shopping s;
   s.menu();

}

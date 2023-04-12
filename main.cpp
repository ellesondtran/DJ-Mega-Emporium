//Elleson Tran
//Roberts
//DJ Mega Emporium 
//manage a very large number of vinyl records that may change at any given time
//FINAL

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//all the information of a vinyl
struct vinyl{
  int sku;
  string title;
  string artist;
  int quantity;
  double price;

  vinyl(){}
  vinyl(int s, string t, string a, int q, double p){
    sku = s;
    title = t;
    artist = a;
    quantity = q;
    price = p;
  }
};

//prints everything in the vinyl list
void report(vector<vinyl> v){
  for(int x = 0; x < v.size(); x++){//goes through each vinyl, prints all the information for each vinyl
    cout<<endl<<"------------------";
    cout<<endl<<v[x].sku<<endl<<v[x].title<<endl<<v[x].artist<<endl<<v[x].quantity<<endl<<"$"<<v[x].price;
  }
  cout<<endl<<"------------------";
}

//adds all the information from the file, into a vector
bool readInRecords(vector<vinyl> &vinyls){
  bool answer;
  vinyl v;

  ifstream infile("inventory.txt");
  if(infile.fail()){//if the file could not be opened, it tells the user, returns false
    cout<<endl<<"File could not be opened!";
    answer = false;
  }
  else{//if it can open, it adds all the information in the file to the vector struct
    while(infile>>v.sku){
      infile.ignore();
      getline(infile, v.title);
      getline(infile, v.artist);
      infile>>v.quantity;
      infile.ignore();
      infile>>v.price;
      infile.ignore();

      vinyls.push_back(v);
    }
  }
  infile.close();//closes after
  return answer;
}

//finds a vinyl that the user wants to find based off the sku number
int searchForReport(vector<vinyl> vinyls, int sku){

  int placement = -1; //what we are returning

  for(int x = 0; x < vinyls.size(); x++){//goes through each vinyl
    if(sku == vinyls[x].sku){//if the sku is found in the vinyls
      placement = x; //the placement is the current vinyl we are on
      return placement;
    }
  }

  //if it goes through all the vinyls, and didnt find the sku, returns -1
  return placement;
}


//prints the sales slip
void printSalesSlip(vector<vinyl> &v, int amount, int placement){
  cout<<"DJ Emporium"<<endl<<"------------------";
  cout<<endl<<"Album:"<<v[placement].title<<endl<<"Price: "<<v[placement].price<<endl<<"Quantity: "<<amount;

  double subtotal = amount * v[placement].price;//subtotal is the price before tax
  cout<<endl<<"Subtotal: "<<subtotal;

  double tax = subtotal * 0.05;//tax is 5%
  cout<<endl<<"Tax: "<<tax;

  double total = tax + subtotal;//total is the albums AND tax included
  cout<<endl<<"Total: "<<total;
}

//lowers the quantity of vinyls if the user wants to sell them
vector<vinyl> sellARecord(vector<vinyl> &v){
  int sku;
  cout<<endl<<"Enter sku to sell: ";
  cin>>sku;
  cin.ignore();

  int placement = searchForReport(v, sku);

  int amount;
  cout<<"How many? ";
  cin>>amount;
  cin.ignore();

  if(placement == -1){//if the album doesnt exist
    cout<<"Sorry, we don't currently carry that album";
    return v;
  }

  if(amount > v[placement].quantity){//if the amount the user wants to sell is greater than the quantity in stock
    cout<<"Sorry, we don't have that many on hand";
    return v;
  }
  else{//if both of those errors dont happen, it goes through
    v[placement].quantity = v[placement].quantity - amount;//updates the quantity
    printSalesSlip(v, amount, placement);//prints the receipt
  }
  return v;
}

//adds a new record or adds to the amount of records
vector<vinyl> addARecord(vector<vinyl> &v){
  int sku;
  cout<<endl<<"Sku: ";
  cin>>sku;
  cin.ignore();

  int placement = searchForReport(v, sku);

  if(placement != -1){//if the album already exists
    cout<<endl<<"Add more stock to an album:"<<endl<<"------------------";
    int amount;
    cout<<endl<<"How many? ";
    cin>>amount;
    cin.ignore();

    v[placement].quantity = v[placement].quantity + amount;//adds more to the quantity
    cout<<"Quantity Added";
  }
  if(placement == -1){//if the album doesnt exist
    cout<<endl<<"Add an Album:"<<endl<<"------------------";
  //adds a new album
    string title;
    cout<<endl<<"Title: ";
    getline(cin, title);

    string artist;
    cout<<"Artist: ";
    getline(cin, artist);

    double price;
    cout<<"Price: $";
    cin>>price;

    int quantity;
    cout<<"Quantity: ";
    cin>>quantity;

    vinyl newPerson(sku, title, artist, quantity, price);//makes the new vinyl
    v.push_back(newPerson);//adds the vinyl to the vector
    cout<<"Album Added";
  }

  return v;

}

//deletes a record
vector<vinyl> deleteARecord(vector<vinyl> &v){
  int sku;
  cout<<endl<<"Delete Album:"<<endl<<"Enter sku to delete: ";
  cin>>sku;

  int placement = searchForReport(v, sku);

  if(placement == -1){//if the record isnt found
    cout<<"We don't have that album";
    return v;
  }
  else{//if the record is found
    v.erase(v.begin() + placement);//uses erase method
    cout<<"Album Deleted";
  }

  return v;
}

//adds all the vinyls to the original file
void writeInventoryToFile(vector<vinyl> &vinyl){
  struct vinyl v;

  ofstream outfile("inventory.txt");
  if(outfile.fail()){//if the file could not be opened, it tells the user, returns false
    cout<<endl<<"File could not be opened!";
  }
  else{//if it can open, it adds all the information in the file to the vector struct
    for(int x = 0; x < vinyl.size(); x++){//goes through each vinyl
      //adds all the info for one vinyl
      int sku = vinyl[x].sku;
      outfile<<sku<<endl;

      string title = vinyl[x].title;
      outfile<<title<<endl;

      string artist = vinyl[x].artist;
      outfile<<artist<<endl;

      int quantity = vinyl[x].quantity;
      outfile<<quantity<<endl;

      double price = vinyl[x].price;
      outfile<<price<<endl;

    }
  }
  outfile.close();//closes after

}


//prints the menu and gets the user choice
int menu(){
  int c;

  vector<string> choices{"Sell a record.", "Add a record to inventory.", "Delete a record from inventory.", "Print record inventory report.", "Exit."};
  
  //prints the menu
  cout<<endl<<endl<<"Main Menu";
  for(int x = 0; x < choices.size(); x++){
    cout<<endl<<x + 1<<")  "<<choices[x];
  }

  //gets the users choice
  cout<<endl<<"Enter a choice: ";
  cin>>c;
  cin.ignore();

  return c;
}

int main() {
  //vector of structs
  vector<vinyl> vinyls;

  //adds the information from the file to the vector^^
  readInRecords(vinyls);
  

  int choice;

  do{//keeps going until the user wants to quit
    choice = menu();

    if(choice == 1){//sell a record
      sellARecord(vinyls);
    }
    if(choice == 2){//add a record from inventory file
        addARecord(vinyls);
    }
    if(choice == 3){//deletes a record from inventory file
      deleteARecord(vinyls);
    }
    if(choice == 4){//prints the inventory
      report(vinyls);
    }
  }while(choice != 5);
  
  writeInventoryToFile(vinyls);
  
}
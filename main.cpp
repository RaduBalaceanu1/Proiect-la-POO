#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>
#include <ctime>

using namespace std;

class Step {
public:
    virtual void execute() = 0;
    virtual void DisplayStepInfo() const = 0;
    virtual void DetailsToFile(std::ofstream& file) const = 0;
    virtual ~Step() = default;
};

class TitleStep: public Step{
private:
    string title;
    string subtitle;
public:
    TitleStep(const string &t, const string &st){
        title = t;
        subtitle = st;
    }
    void execute() override{
        cout << "Executare (title step)\n";
        cout << "Titlu: " << title << "\n";
        cout << "Subtitlu: " << subtitle << "\n";
    }
    void DisplayStepInfo() const override
    {
        cout << "Executare (title step) " << "\n";
        cout << "Titlu: " << title << "\n";
        cout << "Subtitlu: " << subtitle << "\n";
    }

    void DetailsToFile(std::ofstream &file) const override
    {
        file << "Executare (title step) " << "\n";
        file << "Titlu:" << title << "\n";
        file << "Subtitlu: " << subtitle << "\n";
    }
};


class TextStep: public Step
{
private:
    string title;
    string copy;

public:
    TextStep(const string& t, const string& c)
    {
        title = t;
        copy = c;
    }

    void execute() override
    {
        cout << "Executare (text step)\n";
        cout << "Text: " << title << "\n";
        cout << "Copy: " << copy << "\n";
    }


    void DisplayStepInfo() const override
    {
        cout << "Text Step: " << "\n";
        cout << "Text: " << title <<"\n";
        cout << "Copy: "<< copy << "\n";

    }

    void DetailsToFile(ofstream& file) const override
    {
        file << "Text Step: " << "\n";
        file << "Text: " << title << "\n";
        file << "Copy: " << copy << "\n";
    }

};
map<int, float> numberInputValues;

class NumberInputStep: public Step
{
private:
    string description;
    float numberinput;
    int id; // Identificator unic pentru acest pas
    static int idCounter; // Contor static pentru generarea ID-urilor unice

public:
    NumberInputStep(const string& d, float ni){
        description = d;
        numberinput = ni;
        id = ++idCounter; // Incrementați contorul și asignați ID-ul
    }

    void execute() override
    {
        numberInputValues[id] = numberinput;
        cout << "Valoarea introdusa (" << numberinput << ") a fost stocata cu ID-ul " << id << endl;
    }

    void DisplayStepInfo() const override
    {
        cout << "Pasul de introducere numar: " << "\n";
        cout << "Descriere: " << description << "\n";
        cout << "Intoducere numar: " << numberinput << "\n";
        cout << "ID: " << id << "\n";
    }

    void DetailsToFile(ofstream& file) const override
    {
        file << "Pasul de introducere numar: " << "\n";
        file << "Descriere: " << description << "\n";
        file << "Intoducere numar: " << numberinput << "\n";
        file << "ID: " << id << "\n";
    }

    static void resetIdCounter()
    {
        idCounter = 0;
    }
};

int NumberInputStep::idCounter = 0;

map<int, string> textInputValues;  // Pentru TEXT INPUT
map<int, string> textInputDescriptions;  // Pentru descrierile TEXT INPUT


class TextInputStep: public Step
{
private:
    string description;
    string textinput;
    int id; // identificator
    static int idCounter; // Contor static pentru generarea ID-urilor unice

public:
    TextInputStep(const string& d) : description(d), textinput("")
    {
        id = ++idCounter; // Incrementați contorul și asignați ID-ul
    }

    // Metoda execute modificată pentru a cere introducerea textului
    void execute() override
    {
        if (textInputValues.find(id) == textInputValues.end())
        {
            cout << "Introduceti text pentru \"" << description << "\": ";
            getline(cin, textinput);
            textInputValues[id] = textinput; // Stocăm textul introdus în map-ul global
            cout << "Textul introdus \"" << textinput << "\" a fost stocat cu ID-ul " << id << endl;
        }else{
            // Dacă textul a fost deja introdus, îl afișăm
            cout << "Textul pentru \"" << description << "\" este deja introdus: " << textInputValues[id] << endl;

        }
    }

    void DisplayStepInfo() const override
    {
        string  textINPUT;
        cout << "TEXT INPUT Step: " << "\n";
        cout << "Description: " << description << "\n";
        // Verificăm dacă există o valoare în map pentru ID-ul curent
        if (textInputValues.find(id) != textInputValues.end())
        {
            cout << "Text Input: " << textInputValues[id] << "\n";
            getline(std::cin,textINPUT);
        }
        else
        {
            cout << "Text Input: "  << "\n";
            getline(cin,textINPUT);
        }
        cout << "ID: " << id << "\n";
    }


    void DetailsToFile(ofstream& file) const override
    {
        file << "TEXT INPUT Step: " << "\n";
        file << "Description: " << description << "\n";
        // Verificăm dacă există o valoare în map pentru ID-ul curent
        if (textInputValues.find(id) != textInputValues.end()){
            file << "Text Input: " << textInputValues[id] << "\n";
        }else{
            file << "Text Input: " << "[Textul va fi introdus]" << "\n";
        }
        file << "ID: " << id << "\n";
    }
};
int TextInputStep::idCounter = 0; // Inițializare contor ID

class CalculusStep: public Step
{
private:
    vector<int> stepIndices; // Indicii pașilor de tip NUMBER INPUT
    vector<string> operations; // Lista de operații modificate pentru a include min și max

public:
    CalculusStep() {}

    void addStep(int index)
    {
        stepIndices.push_back(index); // Adaugă un indice de pas
    }

    void addOperation(string op)
    {
        operations.push_back(op); // Adaugă un simbol de operație sau cuvinte cheie min/max
    }

    void execute() override
    {
        if (stepIndices.empty() || stepIndices.size() != operations.size() + 1)
        {
            cout << "Numar incorect de pasi sau operatii." << endl;
            return;
        }

        float result = numberInputValues[stepIndices[0]];

        for (size_t i = 0; i < operations.size(); ++i)
        {
            float nextValue = numberInputValues[stepIndices[i + 1]];
            if (operations[i] == "+")
            {
                result += nextValue;
            }
            else if (operations[i] == "-")
            {
                result -= nextValue;
            }
            else if (operations[i] == "*")
            {
                result *= nextValue;
            }
            else if (operations[i] == "/")
            {
                if (nextValue == 0)
                {
                    cout << "Eroare: împartire la zero." << endl;
                    return;
                }
                result /= nextValue;
            }
            else if (operations[i] == "min")
            {
                result = min(result, nextValue);
            }
            else if (operations[i] == "max")
            {
                result = max(result, nextValue);
            }
            else
            {
                cout << "Operatie necunoscuta: " << operations[i] << std::endl;
                return;
            }
        }

        cout << "Rezultatul calculului: " << result << endl;
    }


    void DisplayStepInfo() const override
    {
        cout << "CALCULUS Step:\n";
        cout << "Pasii implicati: ";
        for (const auto& index : stepIndices)
        {
            cout << index << " ";
        }
        cout << "\nOperatii: ";
        for (const auto& op : operations)
        {
            cout << op << " ";
        }
        cout << endl;
    }

    void DetailsToFile(ofstream& file) const override
    {
        file << "CALCULUS Step:\n";
        file << "Pasii implicati: ";
        for (const auto& index : stepIndices)
        {
            file << index << " ";
        }
        file << "\nOperatii: ";
        for (const auto& op : operations)
        {
            file << op << " ";
        }
        file << "\n";
    }
};


class EndStep : public Step
{
public:
    EndStep() {}

    void execute() override
    {
        cout << "This flow has ended.\n" << endl;
    }

    void DisplayStepInfo() const override
    {
        cout << "END Step: This step signifies the end of the flow.\n";
    }

};


class Flow{
private:
    vector<Step*> steps;
    string name;
    string creationTime;
    ///functie pentru a determina timpul
    string getCurrentTimestamp()
    {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        return buf;
    }

public:
    Flow(const string& flowName) : name(flowName){
        /// timpul curent
        creationTime = getCurrentTimestamp();
    }
    void addStep(Step* step){
        steps.push_back(step);
    }
    string getFlowName() const{
        return name;
    }

    void executeFlow(){
        cout<<endl;
        ///numele si ora Fluxului
        cout << "Executarea Fluxului: " << name << endl;
        cout << "Creat la: " << creationTime << endl;

        for (auto& step : steps)
        {
            char alegere;
            cout << "Doriti sa executati urmatorul pas? (d/n): "; //optiunea de a sari peste pasi- cerinta 5
            cin >> alegere;
            if (alegere == 'd' || alegere == 'D')
            {
                step->DisplayStepInfo();  /// Afișează informațiile pasului
                step->execute();          /// Apoi execută pasul
            }else{
                cout << "Sarit un pas...\n\n";
            }
        }
    }

    void saveFlowDetails(const string& filename) const
    {
        ofstream file(filename, ios::app);
        if (!file.is_open())
        {
            cout << "Eroare la deschiderea fisierului: " << filename << endl;
            return;
        }

        file << "Numele fluxului: " << name << "\n";
        for (const auto& step : steps)
        {
            step->DetailsToFile(file);
        }
        file << "------\n";
        file.close();
    }


    ~Flow()
    {
        for (auto& step : steps)
        {
            delete step;
            step = nullptr;
        }
        steps.clear();
    }


};



void displayAvailableSteps(){
    cout << endl;
    cout << "Pasii valabili: " << endl
         << "1. Titlu: adaugarea unui titlu si un subtitlu (string, string)" << endl
         << "2. Text: pas pentru adaugarea unui bloc de text(string)"<< endl
         << "3. Text Input"<<endl
         << "4. Introducere Numerica (string, float) "<<endl
         << "5. Calculus step"<<endl
         << "6. Terminarea fluxului"<<endl;
}

void displayFlows(const vector<Flow>& allFlows)
{
    if (allFlows.empty()) {
        cout << "niciun flux valabil." << endl;
        return;
    }
    cout << "Fluxuri valabile:" << endl;
    for (size_t i = 0; i < allFlows.size(); ++i) {
        cout << i << ": " << allFlows[i].getFlowName() << endl;
    }
}

Step* createStep(){
    int option;
    string title, subtitle, description, textinput, operation, file_name;
    float numberinput;
    int steps;
    cout << endl;
    cout << "Selectati tipul de pas pe care vreti sa-l adaugati: " << endl
         << "1. Titlu (title step) " << endl
         << "2. Text (text step) "<<endl
         << "3. Text Input "<< endl
         << "4. Introducere numar si o descriere(number input step)"<<endl
         << "5. Pas de calculare (calculus step)"<< endl
         << "6. Finalizare"<< endl;

    cout << "Ce optine alegeti";
    cin >> option;
    cin.ignore();

    switch (option) {
        ///TITLE
        case 1:{
            cout << "Introduceti titlul: ";
            getline(cin, title);
            cout << "Introduceti subtitlu: ";
            getline(cin, subtitle);
            return new TitleStep(title, subtitle);
        }
        ///Text
        case 2:{
            cout << "Introduceti titlul: ";
            getline(cin, title);
            cout << "Introduceti o text: ";
            getline(cin, subtitle);
            return new TextStep(title, subtitle);
        }
        ///Text Input
        case 3: {
            cout << "Enter description: ";
            getline(cin, description);
            return new TextInputStep(description);
        }

        ///Number Input
        case 4:
        {
            // Adăugare și execuție NUMBER INPUT Step
            cout << "Introduceti o descriere: ";
            getline(cin, description);
            cout << "Introduceti un numar: ";
            cin >> numberinput;
            cin.ignore();
            auto numberInputStep = new NumberInputStep(description, numberinput);
            numberInputStep->execute(); // Executare imediată
            return numberInputStep;
        }
        /// Pentru pasul CALCULUS
        case 5:
        {
            auto calculusStep = new CalculusStep();
            int numSteps;
            cout << "Introduceti numarul de pasi de tip Number Input pentru calcul: ";
            cin >> numSteps;
            cin.ignore();

            for (int i = 0; i < numSteps; ++i)
            {
                int stepIndex;
                string operation;
                cout << "Introduceti indicele pasului Number Input " << i + 1 << ": ";
                cin >> stepIndex;
                calculusStep->addStep(stepIndex);

                if (i < numSteps - 1)
                {
                    cout << "Introduceti simbolul operatiei (+, -, *, /, min, max) dupa pasul " << i + 1 << ": ";
                    cin.ignore();
                    getline(cin, operation);
                    calculusStep->addOperation(operation);
                }
            }
            calculusStep->execute();
            return calculusStep;
        }

    }

}


int main() {
    vector<Flow> allFlows;
    string flowName;
    char choice;

    while (true) {
        cout << endl;
        cout << "1. Creare flux nou\n";
        cout << "2. Pasii valabili\n";
        cout << "3. Steregerea unui flux" << endl; // Opțiune nouă pentru ștergerea unui flux
        cout << "4. Iesire\n";
        cout << "Alegeti o optione: ";
        cin >> choice;
        cin.ignore();

        if (choice == '1') {
            cout << "Care este numele fluxului: ";
            getline(cin, flowName);
            Flow newFlow(flowName);
            NumberInputStep::resetIdCounter();

            char addMoreSteps;
            do {

                Step *step = createStep();
                if (step != nullptr) {
                    newFlow.addStep(step);
                }

                cout << "Continuati? (d/n): ";
                cin >> addMoreSteps;
                cin.ignore();
            } while (addMoreSteps == 'd' || addMoreSteps == 'D');

            allFlows.push_back(newFlow);

            char runFlow;
            cout << "Vreti sa rulati acest flux? (d/n): ";
            cin >> runFlow;
            if (runFlow == 'd' || runFlow == 'D') {
                newFlow.executeFlow();
            }
        } else if (choice == '2') {
            displayAvailableSteps();
        } else if (choice == '3') {
            displayFlows(allFlows);
            int flowIndex;
            cout << "Care este indexul fluxului pe care doriti sa-l stergeti: ";
            cin >> flowIndex;
            cin.ignore();

            if (flowIndex >= 0 && flowIndex < allFlows.size()) {
                allFlows.erase(allFlows.begin() + flowIndex);
                cout << "Flux sters." << endl;
            } else {
                cout << "Index invalid." << endl;
            }
        } else if (choice == '4') {
            return 0;
        }
    }
}
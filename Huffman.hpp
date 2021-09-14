#ifndef Huffman_h
#define Huffman_h

#include "PriorityQueue.hpp"
#include "HuffmanNode.hpp"
#include "HuffmanCode.hpp"
#include <fstream>

using namespace std;

int parcours = 0;
string decoder;
class Huffman{
private:
    
    static PriorityQueue<HuffmanNode*>* buildKey(string fileContent){
        PriorityQueue<HuffmanNode*>* recensement = new PriorityQueue<HuffmanNode*>();
        HuffmanNode* noeudH;
        char byte;
        char byte2;
        size_t priority;
        bool impression;
        size_t size = fileContent.length();

        for(int i = 0; i < size; i++){
            impression = true;
            byte = fileContent.at(i);
            priority = 0;
            for(int j = 0; j < size; j++){
                byte2 = fileContent.at(j);
                if(byte == byte2)
                    priority ++;
            }
            for(int j = 0; j < i; j++){
                byte2 = fileContent.at(j);
                if(byte == byte2)
                    impression = false;
            }
            if (impression){
                noeudH = new HuffmanNode(byte);
                recensement->push(noeudH, priority);
            }
        }
        return recensement;
    }
    
    static HuffmanNode* buildTree(PriorityQueue<HuffmanNode*>* huffmanQueue){
        HuffmanNode* noeudH;
        size_t priority = 0;
        
        while(huffmanQueue->size()>1){
            noeudH = new HuffmanNode(NULL);
            noeudH->left = huffmanQueue->front();
            priority = huffmanQueue->frontPriority();
            huffmanQueue->pop();
            noeudH->right = huffmanQueue->front();
            priority = priority + huffmanQueue->frontPriority();
            huffmanQueue->pop();
            huffmanQueue->push(noeudH, priority);
            priority = 0;
        }
        noeudH = huffmanQueue->front();
        return noeudH;
    }
    
    static void buildCodes(HuffmanNode* huffmanTree, string code, HuffmanCode** tabHuffmanCode){
        if(huffmanTree->left)
            buildCodes(huffmanTree->left, code + "0", tabHuffmanCode);
        if(huffmanTree->right)
            buildCodes(huffmanTree->right, code + "1", tabHuffmanCode);
        else {
            tabHuffmanCode[parcours] = new HuffmanCode(huffmanTree->data, code);
            parcours++;
        }
    }
    
    static int binaryToDecimal(int n){
        int dec = 0;
        int base = 1;
     
        int temp = n;
        while (temp) {
            int lastDigit = temp % 10;
            temp = temp / 10;
            dec += lastDigit * base;
            base = base * 2;
        }
        return dec;
    }

    static string decimalToBinary(int n){
        int binaryNum[32];
        int i = 0;
        string convertedBinary;

        while (n > 0) {
            binaryNum[i] = n % 2;
            n = n / 2;
            i++;
        }
        for (int j = i - 1; j >= 0; j--)
            convertedBinary = convertedBinary + to_string(binaryNum[j]);

        return convertedBinary;
    }

    static string queueToString(PriorityQueue<HuffmanNode*>* huffmanQueue){
        string conversion;
        while (huffmanQueue->size() > 0) {
            conversion = conversion + huffmanQueue->front()->getData() + " ";
            conversion = conversion + to_string(huffmanQueue->frontPriority()) + " ";
            huffmanQueue->pop();
        }
        conversion = conversion + "~";
        return conversion;
    }

    static void deBuildCodes(HuffmanNode* huffmanTree, string code) {
        if (!huffmanTree->left && !huffmanTree->right)
            decoder = decoder + huffmanTree->data;
        else {
            if (code.at(parcours) = '0') {
                parcours++;
                deBuildCodes(huffmanTree->left, code);
            }
            if (code.at(parcours) = '1') {
                parcours++;
                deBuildCodes(huffmanTree->right, code);
            }
        }
    }
    
public:
    static string encode (string fileContent){
        string code;
        string transfo;
        string conversion;
        size_t grandeur;
        int dec;
        char byte;
        size_t size = fileContent.length();
        
        HuffmanNode* huffmanTree;
        PriorityQueue<HuffmanNode*>* huffmanQueue = Huffman::buildKey(fileContent);
        PriorityQueue<HuffmanNode*>* huffmanQueueCopie = Huffman::buildKey(fileContent);
        grandeur = huffmanQueue->size();
        conversion = queueToString(huffmanQueueCopie);
        huffmanTree = Huffman::buildTree(huffmanQueue);
        HuffmanCode** tabHuffmanCode = new HuffmanCode*[grandeur];
        delete huffmanQueue;
        Huffman::buildCodes(huffmanTree, code, tabHuffmanCode);

        parcours = 0;

        //Création du code binaire dans la string code
        for(size_t i = 0; i < size; i++){
            byte = fileContent.at(i);

            while (byte != tabHuffmanCode[parcours]->data)
                parcours++;
            
            code.append(tabHuffmanCode[parcours]->code);
            parcours = 0;
        }
        
        //Définir les octets et ajouter les bits de bourrages
        for(size_t i=0; i<code.size() % 8 ; i++)
            code.append("0");
        
        code.insert(8, " ");
        for(size_t i=9; i<code.size();i++){
            if (i % 8 == 0) {
                parcours++;
                code.insert(i + parcours, " ");
            }
        }
        
        //Transformer le code binaire en décimal
        for(size_t i=0; i< code.size(); i++){
            if(code.at(i) == ' '){
                dec = binaryToDecimal(stoi(transfo));
                transfo = to_string(dec);
                conversion.append(transfo + ' ');
                transfo = "";
            }
            else{
                byte = code.at(i);
                transfo.append(1, byte);
            }
        }
        return conversion;
    };
    static string decode (string fileContent){
        string code;
        string transfo;
        string conversion;
        size_t grandeur;
        char byte;
        size_t size = fileContent.length();
        parcours = 0;
        PriorityQueue<HuffmanNode*>* huffmanQueue = new PriorityQueue<HuffmanNode*>();
        HuffmanNode* noeudH;
        HuffmanNode* huffmanTree;

        while (!(fileContent.at(parcours) == '~')) {
            noeudH = new HuffmanNode(fileContent.at(parcours));
            parcours = parcours + 2;
            while (!(fileContent.at(parcours) == ' ')) {
                transfo = transfo + fileContent.at(parcours);
                parcours++;
            }
            huffmanQueue->push(noeudH,stoi(transfo));
            parcours++;
            transfo = "";
        } 

        huffmanTree = Huffman::buildTree(huffmanQueue);

        //Décimal vers binaire en octet
        for (size_t i = parcours + 1; i < size; i++) {
            if (fileContent.at(i) == ' ') {
                transfo = decimalToBinary(stoi(transfo));
                while (transfo.length() < 8)
                    transfo = transfo + "0";
                conversion.append(transfo + ' ');
                transfo = "";
            }
            else {
                byte = fileContent.at(i);
                transfo.append(1, byte);
            }
        }
        parcours = 0;
        Huffman::deBuildCodes(huffmanTree, conversion);

        return decoder;

    }
};

#endif
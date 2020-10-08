/*
  PontoCuboLED

  Esse programa acende um unico LED em um cubo de LED's 4x4x4, movimentando ele por todo
  o cubo de maneira aleatória. Há um gif demonstrativo do funcionamento desse programa 
  na pasta do GitHub onde está este código, no arquivo README.md

  O cubo de LED's foi abstraido da seguinte forma:
  - As 16 saídas referentes às colunas são tratadas como uma matriz 4x4;
  - As 4 camadas do cubo estão representadas em um único vetor de 4 posições;

  O ponto pode se movimentar em 3 dimensões de acordo com os critérios abaixo:
  - De uma camada para a outra (acima ou abaixo);
  - Em uma camada para esquerda, direita ou em frente;

  Dentro loop principal, a cada "volta" o programa sorteia um movimento dentre as opções 
  descritas anteriormente, isso da o aspecto aleatório à movimentação. Há diversas regras 
  para tornar a movimentação mais natural e mais aleatória, por isso a longa cadeia de 
  comando de decisão.

  Utilizei um Arduino Nano nesse projeto.
  
  Modificado em 8 de outubro de 2020
  Por Fabio Mendes
*/

//Tempo entre um movimento e outro
#define TEMPO  400

//Matriz de abstração dos pinos das colunas
int matriz[4][4] = { { 1,  5,  9,  13},
                     {15,  4,  8,  12},
                     {14,  3,  7,  11},
                     { 0,  2,  6,  10} };

/*A variáveis abaixo são usadas na movimentação do ponto e restrições 
de movimentação
LC: O ponto sempre vai iniciar na linha 0, coluna 0 e camada 3;
aux: usada para aramazenar o numero sorteado;
antLouC: informa se o movimento anterior foi de movimentação na linha 
         ou coluna
antIouD: usada para manter o controle se a movimentação anterior de incremento ou
         decremento;
operAtual: Armazena se a operação atual é de incremento ou decremento.*/
int LC[3] = {0, 0, 3}; //Linha, Coluna e Camada
int aux=0;
int antLouC;
bool antIouD=false;
bool operAtual;

// camadas de LED's
int camada[] = {16,17,18,19};

void setup() {
  // Configurando as colunas de LED's como saídas
  for(int linha = 0; linha < 4; linha++){
    for(int coluna = 0; coluna < 4; coluna++){
      pinMode(matriz[linha][coluna], OUTPUT);
      digitalWrite(matriz[linha][coluna], LOW);
    }
  }

  // Configurando as camadas de LED's como saídas
  for(int layer = 0; layer < 4; layer++){
    pinMode(camada[layer], OUTPUT);
    digitalWrite(camada[layer], LOW);
  }

//Atribuindo randomSeed a um pino analógico para que a cada execução
//o npumero sorteado seja diferente
  randomSeed( analogRead(A6) );
}

void loop() {
  //posições iniciais do ponto
  digitalWrite(camada[ LC[2] ], HIGH);
  digitalWrite(matriz[ LC[0] ][ LC[1] ], HIGH);
  delay(TEMPO);
  digitalWrite(matriz[ LC[0] ][ LC[1] ], LOW);
  digitalWrite(camada[ LC[2] ], LOW);
  
  aux = random(3); //Sorteio se o 1° movimento será de linha, coluna ou Altura
  if(random(2) == 0){ //Sorteio uma operação, que se for de decremento...
    if( LC[aux] > 0 ){
      LC[aux]--; //Posso decrementar...
      antIouD = false;
    }
    else{
      LC[aux]++; //Não posso decrementar, faço incremento
      antIouD = true;
    }
  }
  else{ //O sorteio deu incremento
    if( LC[aux] < 3 ){
      LC[aux]++; //Posso incrementar
      antIouD = true;
    }
    else{
      LC[aux]--; //Não posso incrementar, então faço decremento
      antIouD = false;
    }
  }

  //Acende o LED
  digitalWrite(camada[ LC[2] ], HIGH);
  digitalWrite(matriz[ LC[0] ][ LC[1] ], HIGH);
  delay(TEMPO);
  digitalWrite(matriz[ LC[0] ][ LC[1] ], LOW);
  digitalWrite(camada[ LC[2] ], LOW);
  
  while(1){
  
  antLouC = aux;
  aux = random(3); //Sorteio Linha, Coluna ou Camada
  
  if( aux == antLouC ){ //O anterior foi o mesmo que agora (seja L ou C)
    operAtual = random(2);
    if( operAtual == antIouD ){ //A operação sorteada agora é a mesma que a anterior (seja decremento ou incremento)?
      //Sim, é a mesma
      if( operAtual==false ){ //Essa operação é de decremento?
        if( LC[aux] > 0 ){
          LC[aux]--; //Se posso decrementar faço isso
          antIouD = false;
        }
        else{ //Não posso decrementar
          if(aux == 0) aux++; //Troco de linha para coluna
          else aux--; //Troco de coluna para linha
          operAtual = random(2); //Sorteio uma nova operação
          if(operAtual==false){ //A operação foi de decremento?
            if(LC[aux] > 0){
              LC[aux]--;
              antIouD = false;
            }
            else{
              LC[aux]++;
              antIouD = true;
            }
          }
          else{ //A operação foi de incremento
            if(LC[aux] < 3){
              LC[aux]++;
              antIouD = true;
            }
            else{
              LC[aux]--;
              antIouD = false;
            }
          }
        }
      }
    }
    else{ //A operação sorteada agora è diferente da anterior (inc. ou dec.) para a mesma linha ou coluna
      if( random(2) ){ //Teste de V ou F: V = tentar refazer a operação anterior na mesma linha ou coluna
        if( antIouD == false ){ //A operação anterior foi de decremento
          if( LC[aux] > 0){
            LC[aux]--;
            antIouD = false;
          }
          else{
            if(aux == 0) aux++; //Troco de linha para coluna
            else aux--; //Troco de coluna para linha
            operAtual = random(2); //Sorteio uma operação
            if(operAtual == 0){ //A operação sorteada foi decremento
              if( LC[aux] > 0 ){
                LC[aux]--;
                antIouD = false;
              }
              else{
                LC[aux]++;
                antIouD = true;
              }
            }
            else{ //A operação sorteada foi incremento
              if( LC[aux] < 3){
                LC[aux]++;
                antIouD = true;
              }
              else{
                LC[aux]--;
                antIouD = false;
              }
            }
          }
        }
        else{ //A operação anterior foi de incremento
          if( LC[aux] < 3 ){
            LC[aux]++;
            antIouD = true;
          }
          else{
            if(aux == 0) aux++; //Troco de linha para coluna
            else aux--; //Troco de coluna para linha
            operAtual = random(2); //Sorteio uma operação
            if(operAtual == 0){ //A operação sorteada foi decremento
              if( LC[aux] > 0 ){
                LC[aux]--;
                antIouD = false;
              }
              else{
                LC[aux]++;
                antIouD = true;
              }
            }
            else{ //A operação sorteada foi incremento
              if( LC[aux] < 3){
                LC[aux]++;
                antIouD = true;
              }
              else{
                LC[aux]--;
                antIouD = false;
              }
            }
          }
        }
      }
      else{ //Teste de V ou F: F = trocar L por C ou C por L ou trocar para andar
        switch(aux){
          case 0:
            if( random(2) ) aux++;
            else aux =+ 2;
          break;
          case 1:
            if ( random(2) ) aux++;
            else aux--;
          break;
          case 2:
            if ( random(2) ) aux--;
            else aux =- 2;
          break;
        }
        operAtual = random(2); //Sorteio uma operação aleatória
        if(operAtual == false){ //A operação sorteada foi de decremento
          if( LC[aux] > 0 ){
            LC[aux]--;
            antIouD = false;
          }
          else{
            LC[aux]++;
            antIouD = true;
          }
        }
        else{ //A operação sorteada foi de incremento
          if( LC[aux] < 3 ){
            LC[aux]++;
            antIouD = true;
          }
          else{
            LC[aux]--;
            antIouD = false;
          }
        }
      }
    }
  }
  else{ //O anterior foi diferente do de agora (seja L ou C)
    if( random(2) == 0 ){ //Sorteei decremento
      if( LC[aux] > 0 ){
        LC[aux]--; //Se posso, então decremento
        antIouD = false;
      }
      else{
        LC[aux]++; //Não posso decrementar, então é feito incremento
        antIouD = true;
      }
    }
    else{ //Sorteio deu incremento
      if( LC[aux] < 3){
        LC[aux]++; //Se posso incrementar, então decremento
        antIouD = true;
      }
      else{
        LC[aux]--; //Se não posso incrementar, faço decremento
        antIouD = false;
      }
    }
  }

  //Acende o LED
  digitalWrite(camada[ LC[2] ], HIGH);
  digitalWrite(matriz[ LC[0] ][ LC[1] ], HIGH);
  delay(TEMPO);
  digitalWrite(matriz[ LC[0] ][ LC[1] ], LOW);
  digitalWrite(camada[ LC[2] ], LOW);
  
  } //while
} //void loop

# projeto_ihs

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.
This project was made using Quartus II 15.0 (64-bit) and an Altera DE2-115.
The specification and tutorials can be found in the pdf papers in this repository.

## Prerequisites

First clone this repository

```
git clone https://github.com/zsmn/projeto_ihs.git
```

You also will need:

```
allegro >= 5
```

### Installing Allegro

Now after clone the repository you will need to install allegro5 

```
sudo apt-get update
sudo apt-get install liballegro5-dev
```

## How to run

First you need to run the map in the Altera DE2-115, that can be found in the .zip in this repository.
After mapping, you can now run the program (in the altera DE2-115)
First open the "driver" folder and run this command on your terminal:

```
sudo sh desg.sh  
```

After that, open the "app" folder and tap this command on your terminal:

```
sudo make all # to make all games (genius, pong and minefield)
sudo make app1 # to make only app1 (genius game)
sudo make app2 # to make only app2 (pong game)
sudo make app3 # to make only app3 (minefield game)
```

Note that this project have three apps that you can run (genius, pong and minefield), and you can decide them in the makefile!
Now after compiling the app, you can now run it with that command:

```
sudo ./app1 # to run genius game
sudo ./app2 # to run pong game
sudo ./app3 # to run minefield game
```

#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>



using namespace std;

void 	InitArray	 	(char **arr, int column, int line);
void 	Recive_pic		();
char 	**GetArray		(int column, int line);
int 	test_2_2		(char **arr, int column, int line); //не нужен
int 	test_2_3		(char **arr, int column, int line);


int main(int argc, char* argv[])
{
	cout << "------------------------------------------------\n";
	
	char buff[2]; // буфер для "Р2"
	int column, line; // стоблец и строка массива 
	int NUM_of_lines=0; //количество линий в картинке
	ifstream file("ok.pbm");

	if (!file.is_open()) // если файл не открыт, то печатаем ошибку 
        cout << "Файл не может быть открыт!\n"; 
	else
		{	
			cout << "Файл открыт успешно!\n";
			file >> buff;										// Р2
			cout << buff << "\n";
			file >> column >> line;
			cout << column << " столбцов и " << line << " строк \n"; 				// вытаскиваем размер столбца и строки

			char **Pic_array = GetArray(column, line);
			InitArray(Pic_array, column, line);
			
			for (int ln=0; ln < line; ln++)
				{
					for (int col=0; col < column; col++)	
					{
						file >> Pic_array[ln][col]; //вытаскиваем значения из потока в массив 
						//cout << Pic_array[ln][col]; // распечатываем элемент массива								
					}	
				}
			NUM_of_lines = test_2_3(Pic_array, column, line);					
		}
		
	cout << "количество линий= " << NUM_of_lines << "\n";	
	cout << "Закрываем файл...\n";
	
	file.close();
	
	cout << "Нажмите Enter для завершения программы\n";
	cout << "------------------------------------------------\n";
	cin.get(); //аналог readln()
	
	return 0;
}

int test_2_2 		(char **arr, int column, int line)
{
	int count=0;
	int NUM_of_lines=0;
	
	for (int col=0; col<column; col++)
	{
		for (int ln=0; ln<line;ln++)
		{
			if (arr[ln][col]=='1')
				count++;
		}
		
		if (count>NUM_of_lines) 
		{
			NUM_of_lines=count;
		}
	count=0;
	}
	
	return NUM_of_lines;
}

int test_2_3 		(char **arr, int column, int line)
{
	int NUM_of_lines=0;
	int flag=0;
	int Ct_column=0;
	
	for (int ln=1; ln<line; ln++)
	{
		for (int col=0; col<column; col++)
		{
			if (arr[ln][col]!='1' && arr[ln-1][col]!='1' && flag==0) Ct_column++;
			else 
			if (arr[ln][col]=='1') flag=0;
		}
		
		if (Ct_column==column) 
		{
			NUM_of_lines++;
			flag=1;
		} 
		
		Ct_column=0; 
	}
	
	for (int col=0; col<column; col++) //Блок проверки последней строки. Если есть там 1, то NUM_of_lines+1
	{
		if (arr[line-1][col]=='1' || arr[line-2][col]=='1')
		{
			NUM_of_lines++;
			break;
		}
	}
	
	return NUM_of_lines;
}

char **GetArray	 	(int column, int line)
{
	int a,b;
	if (column > line){a=column; b=line;} else {a=line; b=column;} 
	char **Pic_array = new char* [a];				// ----------- уточнить по поводу задания массива, нужно сначала большее, а потом меньшее, а ли без разницы? (тесты показывают, что есть разница, но все же стоит уточнить...)
		for (int count=0; count < a; count++)		// Блок создания динамического х2 массива
			Pic_array[count]= new char [b];			// ------------
	
	cout << "Создается динамический массив...\n";
	return Pic_array;
}

void InitArray	 (char **arr, int column, int line)
{
		cout << "Выполнятется InitArray...\n";
		for (int i; i < line; i++)
		{
			for (int j=0; j < column; j++)
			{
				arr[i][j]='0';
				cout << arr[i][j];
			}
			cout << "\n";
		}	
		cout << "Готово \n";
		
}


void Recive_pic	() //в разработке
{
	
	char reseved_size[10];
	int column=0, line=0;
    int sockfd, newsockfd, portno, n;
	char buffer[256];
	char portno_chr[5];
    struct sockaddr_in serv_addr, cli_addr;
	
	socklen_t clilen;
   
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
	if (sockfd < 0) 
		cout << "ERROR opening socket";
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	cout << "Введите номер порта \n";
	gets(portno_chr); //заменить на cin.get();
    portno = atoi(portno_chr); //считываем и переводим
	
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
	
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		cout << "ERROR on binding";
	
	listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
	
    if (newsockfd < 0) 
		cout << "ERROR on accept";
    
	bzero(reseved_size,10);
	
    n = read(newsockfd,reseved_size,10);
	if (n < 0) 
	{
		cout << "ERROR reading from socket"; 
		exit(1);
	}
	
    cout << "Размеры: " << reseved_size;
	
	/*
	пихнуть сюда обработку reseved_size -> column && line;
	*/
	
    /*n = write(newsockfd,"Теперь, отправь строку",22);
	
	if (n < 0) 
	{
		cout << "ERROR writing to socket"; 
		exit(1);
	}
	*/
	
	//----------------------------------
	int size_of_array=column*line+column;
	char reseved_line[size_of_array];
    n = read(newsockfd, reseved_line, size_of_array);
		if (n < 0) 
	{
		cout << "ERROR reading from socket"; 
		exit(1);
	}
	
    cout << "строка принята";
	//обработка массива и дальнейшее сканирование
	/* n = write(newsockfd,"Merci beaucoup!",15);
	if (n < 0) 
	{
		cout << "ERROR writing to socket"; 
		exit(1);
	}
	*/
	
	//close(newsockfd);
    close(sockfd);
}

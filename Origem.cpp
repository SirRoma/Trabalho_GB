#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;
using namespace std;

//Var. globais
Rect efeitoup, efeitodown, trocaup, trocadown, salvar;
Mat3b canvas;
string buttonInit1("Carregar Imagem"), buttonInit2("Abrir Camera"),winName = "Selecao", winName2 = "Figuras";
int escolha, filtro = 0, valor = 50, stick = 1, stx = -1, sty = -1, cliques = 0;
//Var. principal
Mat imageOut;

//Stickers
Mat stick1 = imread("img/stick1.png", IMREAD_UNCHANGED);
Mat stick2 = imread("img/stick2.png", IMREAD_UNCHANGED);
Mat stick3 = imread("img/stick3.png", IMREAD_UNCHANGED);
Mat stick4 = imread("img/stick4.png", IMREAD_UNCHANGED);
Mat stick5 = imread("img/stick5.png", IMREAD_UNCHANGED);

//Funções
Mat aplicaEfeito(Mat image);
void alteraValores();
void callBackFunc(int event, int x, int y, int flags, void* userdata);
void overlayImage(Mat* image, Mat* stick, const Point& location);

int main()
{
	//Var. principal
	Mat image;

	//Transforma em canais com Alpha
	cvtColor(stick1, stick1, COLOR_BGR2BGRA);
	cvtColor(stick2, stick2, COLOR_BGR2BGRA);
	cvtColor(stick3, stick3, COLOR_BGR2BGRA);
	cvtColor(stick4, stick4, COLOR_BGR2BGRA);
	cvtColor(stick5, stick5, COLOR_BGR2BGRA);

	//Redimensiona 50x50
	resize(stick1, stick1, Size(50, 50), 0.2, 0.2);
	resize(stick2, stick2, Size(50, 50), 0.2, 0.2);
	resize(stick3, stick3, Size(50, 50), 0.2, 0.2);
	resize(stick4, stick4, Size(50, 50), 0.2, 0.2);
	resize(stick5, stick5, Size(50, 50), 0.2, 0.2);
	
	VideoCapture capture;
	escolha = -1;
	bool valido = false, imgValido = false, clear = false;

	while (!valido) {
		system("cls");
		cout << "MENU" << endl;
		cout << "1-Carregar imagem!" << endl;
		cout << "2-Abrir camera!" << endl;
		cin >> escolha;
		alteraValores();

		switch (escolha) {

		case 1:

			valido = true;
			image = imread("img/pikachu.png", IMREAD_UNCHANGED);
			if (!image.data)
			{
				cout << "Could not open or find the image" << std::endl;
				return -1;
			}
			
			cvtColor(image, image, COLOR_BGR2BGRA);

			imageOut = image;
			while (!imgValido) {
				imshow(winName2, aplicaEfeito(imageOut));
				setMouseCallback(winName2, callBackFunc);
				if ((stx != 0) && (sty != 0)&&(cliques == 1)) {
					switch (stick) {
					case 1:
						overlayImage(&image, &stick1, Point(stx - 25, sty - 25));
						stick++;
						break;
					case 2:
						overlayImage(&image, &stick2, Point(stx - 25, sty - 25));
						stick++;
						break;
					case 3:
						overlayImage(&image, &stick3, Point(stx - 25, sty - 25));
						stick++;
						break;
					case 4:
						overlayImage(&image, &stick4, Point(stx - 25, sty - 25));
						stick++;
						break;
					case 5:
						overlayImage(&image, &stick5, Point(stx - 25, sty - 25));
						stick = 1;
						break;
					default:
						break;
					}
					cliques = 0;
				}
				waitKey(300);

				if (!clear) {
					system("cls");
					cout << "Camera aberta" << endl;
					clear = true;
					cout << "Efeito a ser aplicado:" << endl;
					cout << "0-Normal" << endl;
					cout << "1-Cinza" << endl;
					cout << "2-Gaussiano" << endl;
					cout << "3-Canny" << endl;
					cout << "4-Gaussiano + Canny" << endl;
					cout << "5-Sobel" << endl;
					cout << "6-Inverte cores" << endl;
					cout << "7-Inverte cores cinza" << endl;
					cout << "8-Nao sei (BGR2Luv)" << endl;
					cout << "9-TV com estatica" << endl;
					cout << "10-Sepia" << endl;
				}
			}
				break;
		case 2:
			valido = true;
			capture.open(0);
			if (capture.isOpened())
			{
				clear = false;
				for (;;)
				{
					capture >> image;
					if (image.empty())
					break;
					//drawText(image);
					flip(image,imageOut,1);
					imshow(winName2, aplicaEfeito(imageOut));
					if (waitKey(10) >= 0)
						break;
					if (!clear) {
						system("cls");
						cout << "Camera aberta" << endl;
						clear = true;
						cout << "Efeito a ser aplicado:" << endl;
						cout << "0-Normal" << endl;
						cout << "1-Cinza" << endl;
						cout << "2-Gaussiano" << endl;
						cout << "3-Canny" << endl;
						cout << "4-Gaussiano + Canny" << endl;
						cout << "5-Sobel" << endl;
						cout << "6-Inverte cores" << endl;
						cout << "7-Inverte cores cinza" << endl;
						cout << "8-Nao sei (BGR2Luv)" << endl;
						cout << "9-TV com estatica" << endl;
						cout << "10-Sepia" << endl;
					}
				}
			}
			else
			{
				cout << "No capture" << endl;
				image = Mat::zeros(480, 640, CV_8UC1);
				
				imshow("Sample", image);
				waitKey(0);
			}
			break;

		default:
			system("cls");
			cout << "Digite uma opcao valida" << endl;
			break;
		}
	}
	system("cls");
	
	return 0;
}

void alteraValores() {

	//Posição botões
	efeitoup = Rect(0, 0, 250, 250);
	efeitodown = Rect(251, 0, 250, 250);
	trocaup = Rect(0, 251, 250, 250);
	trocadown = Rect(251, 251, 250, 250);
	salvar = Rect(0, 502, 250, 250);

	//Tamanho canvas
	canvas = Mat3b(752, 502, Vec3b(0, 0, 0));

	//Tamanho botões no canvas
	canvas(efeitoup) = Vec3b(200, 200, 200);
	canvas(efeitodown) = Vec3b(200, 200, 200);
	canvas(trocaup) = Vec3b(200, 200, 200);
	canvas(trocadown) = Vec3b(200, 200, 200);
	canvas(salvar) = Vec3b(200, 200, 200);

	//Texto e tamanho botões
	putText(canvas(trocaup), "Prox Efeito", Point(trocaup.width * 0.1, trocaup.height * 0.1), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0));
	putText(canvas(trocadown), "Efeito anterior", Point(trocadown.width * 0.1, trocadown.height * 0.1), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0));
	putText(canvas(efeitoup), "Efeito +", Point(efeitoup.width * 0.1, efeitoup.height * 0.1), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0));
	putText(canvas(efeitodown), "Efeito -", Point(efeitodown.width * 0.1, efeitodown.height * 0.1), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0));
	putText(canvas(salvar), "Salvar", Point(salvar.width * 0.1, salvar.height * 0.1), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0));

	namedWindow(winName);
	setMouseCallback(winName, callBackFunc);

	imshow(winName, canvas);
}

void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (trocaup.contains(Point(x, y)))
		{
			if (filtro <= 9) {
				filtro++;
			}
			cout << "Filtro: " << filtro << endl;
			rectangle(canvas(trocaup), trocaup, Scalar(0, 0, 255), 2);
		}
		if (trocadown.contains(Point(x, y)))
		{
			if (filtro >= 1) {
				filtro--;
			}
			cout << "Filtro: " << filtro << endl;
			rectangle(canvas(trocadown), trocadown, Scalar(0, 0, 255), 2);
		}
		if (efeitoup.contains(Point(x, y)))
		{
			if (valor <= 90) {
				valor += 10;
			}
			cout << "Efeito: "<< valor<<"%" << endl;
			rectangle(canvas(efeitoup), efeitoup, Scalar(0, 0, 255), 2);
		}
		if (efeitodown.contains(Point(x, y)))
		{
			if (valor >= 10) {
				valor -= 10;
			}
			cout << "Efeito: " << valor << "%" << endl;
			rectangle(canvas(efeitodown), efeitodown, Scalar(0, 0, 255), 2);
		}
		if (salvar.contains(Point(x, y)))
		{
			imwrite(".teste.jpg", aplicaEfeito(imageOut));
		}

	}
	if (event == EVENT_RBUTTONDOWN) {
		stx = x;
		sty = y;
		cliques = 1;
	}

	if (event == EVENT_LBUTTONUP)
	{
		//rectangle(canvas, button, Scalar(200, 200, 200), 2);
	}
	imshow(winName, canvas);
	waitKey(1);
}

void overlayImage(Mat* image, Mat* stick, const Point& location)
{
	for (int y = max(location.y, 0); y < image->rows; ++y)
	{
		int fY = y - location.y;
		if (fY >= stick->rows)
			break;
		for (int x = max(location.x, 0); x < image->cols; ++x)
		{
			int fX = x - location.x;
			if (fX >= stick->cols)
				break;
			double opacity = ((double)stick->data[fY * stick->step + fX * stick->channels() + 3]) / 255;
			for (int c = 0; opacity > 0 && c < image->channels(); ++c)
			{
				unsigned char stickPx = stick->data[fY * stick->step + fX * stick->channels() + c];
				unsigned char imagePx = image->data[y * image->step + x * image->channels() + c];
				image->data[y * image->step + image->channels() * x + c] = imagePx * (1. - opacity) + stickPx * opacity;
			}
		}
	}
}

Mat aplicaEfeito(Mat image) {
	Mat imageOut, aux, aux2, kernel;

	switch (filtro) {
	case 0:
		return image;
		break;
	case 1://Cinza
		cvtColor(image, imageOut, COLOR_BGR2GRAY);
		break;
	case 2://GaussianBlur
		GaussianBlur(image, imageOut, Size(15, 15), (valor/10), 0);
		break;
	case 3://Canny
		Canny(image, imageOut, valor/2, valor/2);
		break;
	case 4: //Cany + Blur
		GaussianBlur(image, aux, Size(15, 15), (valor / 10), 0);
		Canny(aux, imageOut, valor / 2, valor / 2);
		break;
	case 5:
		medianBlur(image, imageOut, 11);
;		break;
	case 6:
		bitwise_not(image, imageOut);
		break;
	case 7:
		cvtColor(image, aux, COLOR_BGR2GRAY);
		bitwise_not(aux, imageOut);
		break;
	case 8:
		cvtColor(image, imageOut, COLOR_BGR2Luv);
		break;
	case 9:
		cvtColor(image, aux, COLOR_RGB2RGBA);
		kernel = (Mat_<float>(4, 4)
			<<
			0.0, 0.0, 1, 0,
			0.0, 0.0, 1, 0,
			0.0, 0.0, 1, 0,
			0, 0, 0, 1);
		transform(aux, imageOut, kernel);
		break;
	case 10:
		cvtColor(image, aux, COLOR_BGR2GRAY);
		cvtColor(aux, aux2, COLOR_RGB2RGBA);
		kernel = (Mat_<float>(4, 4)
			<<
			0.272, 0.534, 0.131, 0,
			0.349, 0.686, 0.168, 0,
			0.393, 0.769, 0.189, 0,
			0, 0, 0, 1);
		transform(aux2, imageOut, kernel);
		break;
	default:
		cout << "Sem opcao para isto" << endl;
		break;
	}
	return imageOut;
}

Mat aplicaStick(Mat image) {
	Mat imageOut, aux, aux2, kernel;

	return imageOut;
}
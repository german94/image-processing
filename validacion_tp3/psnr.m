%%La funcion toma las string de las dos imagenes
function PSNR = psnr(img1, img2);
	I1 = imread(img1);
	I2 = imread(img2);	
	filas = size(I1,1);
	columnas = size(I1,2); 
	ecmImg = (double(I1(:,:)) - double(I2(:,:))) .^ 2;	
	ecm = sum(sum(ecmImg)) / (filas * columnas);	
	PSNR = 10 * log10( 255^2 / ecm);

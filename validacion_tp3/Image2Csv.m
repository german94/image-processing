%Para ejecutar en consola: matlab -nodisplay -nosplash -nojvm -r "Image2Txt('imagen.tiff');quit" ó simplemente matlab -r "Image2Txt('imagen.tiff');quit" creo que lo parametros demás es para que no se abra el programa.
function Image2Csv(str)
f = imread (str);       		%leo la imagen 
engrisf = rgb2gray(f);  		%la paso a gris
s = strcat(str, '.csv');  		%concateno la imagen con la extension csv, por ejemplo me quedaria lena.bmp.csv
s1 = strcat(str, '.gris.tiff');
imwrite(engrisf, s1);			%creo la imagen que esta en gris
dlmwrite(s, engrisf, ' ')		%guardo la matriz de la imagen en gris
end

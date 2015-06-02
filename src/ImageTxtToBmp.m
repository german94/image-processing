function ImageTxtToBmp(f, i)
A= uint8(load(f)); %cargo la imagen como una matriz
imwrite(A, i); %creo la imagen que esta en gris
end

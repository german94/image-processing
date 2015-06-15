function Mret = reducir(f, k);
M = uint8(imread(f)); 
i = 1;
Mret = [];
n=size(M, 1);
m= size(M, 2);
kk= str2num(k)
in=1;
while (i <= n)
	j = 1;
	im=1;
    while (j <= m)
		Mret(in, im, 1) = M(i, j, 1);
		Mret(in,  im, 2) = M(i, j, 2);
		Mret(in,  im , 3) = M(i, j, 3);
        j = j + kk +1
        im= im +1
    end
   i = i + kk + 1;
   in= in+1;
end
imwrite(uint8(Mret), strcat(f, '_REDUCIDA.tiff'));

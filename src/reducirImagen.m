function Mret = reducir(f, k);
M = uint8(imread(f)); 
i = 1;
Mret = [];
while i <= size(M, 1)
    j = 1;
    while j <= size(M, 2)
        Mret(1 + (i - 1)/(k + 1),   1 + (j - 1)/(k + 1), 1) = M(i, j, 1);
	Mret(1 + (i - 1)/(k + 1),   1 + (j - 1)/(k + 1), 2) = M(i, j, 2);
	Mret(1 + (i - 1)/(k + 1),   1 + (j - 1)/(k + 1), 3) = M(i, j, 3);
        j = j + k + 1;
    end
    i = i + k + 1;
end
imwrite(uint8(Mret), strcat(f, '_REDUCIDA.bmp'));

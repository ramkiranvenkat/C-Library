function y = PDM(A)
s = size(A,1);
str = 1;
epsPDM = 1e-15;
y(str) = 1;
for i=1:s-1

	if ((norm(A(i+1:s,1:i)) < epsPDM) && (norm(A(1:i,i+1:s)) < epsPDM))
		str = str + 1;
		y(str) = i+1;
	end
end

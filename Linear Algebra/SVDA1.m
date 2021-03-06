function [U S V] = SVDA1(inp)
A = inp;

QLi = eye(size(A,1));
QRi = eye(size(A,2));

epsilon = 1e-16;
errorVal = norm(A);
errorValp = 0;
errorDiff = errorVal - errorValp;

while ((errorVal > epsilon) && (errorDiff > 5e-16))
	[Ql R] = hQR(A);
	[L Qr] = hLQ(R);
	A = L;
	QLi = QLi*Ql;
	QRi = Qr*QRi;

	errorVal = 0;
	for i=1:size(A,1)
		for j=1:size(A,2)
			if (i~=j)
				errorVal = errorVal + A(i,j)*A(i,j);
			end	
		 end
	end
	errorVal  = sqrt(errorVal);
	errorDiff = abs(errorVal - errorValp);
	errorValp = errorVal;
end

for i=1:size(A,1)
	for j=1:size(A,2)
		if (i~=j)
			A(i,j) = 0;
		end
	end
end

S = A;
U = QLi;
V = QRi';


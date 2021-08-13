function [y Qout] = SCHUR(A);
[A QHess]= HESS(A);
epsilon = 1e-15;
Ac = A;
s1 = size(A,1);
s2 = size(A,2);
An = 0*eye(s1,s2);
errorVal = norm(An-Ac);
errorValp = 0;
errorDiff = errorVal - errorValp;
Qc = eye(s1,s2);

while ((errorVal > epsilon) && (errorDiff > 5e-16))
	Z = Ac*Qc;
	[Qc R] = hQR(Z);
	An = Qc'*Ac*Qc;
	errorVal = 0;
	for i=1:s1
		for j=1:s2
			if (abs(An(i,j)) < 1e-32)
				An(i,j) = 0;
			end
			if (i<j)
				errorVal = errorVal + An(i,j)*An(i,j);
			end	
		 end
	end
	errorVal  = sqrt(errorVal);
	errorDiff = abs(errorVal - errorValp);
	errorValp = errorVal;
end 	
Qout = Qc'*QHess;
y = An;

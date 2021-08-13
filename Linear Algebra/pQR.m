function y = pQR(A)
[A QHess]= HESS(A);
epsilon = 1e-15;
Ao = A;
s1 = size(A,1);
s2 = size(A,2);
An = 0*eye(s1,s2);
errorVal = norm(An-Ao);
errorValp = 0;
errorDiff = errorVal - errorValp;

while ((errorVal > epsilon) && (errorDiff > 5e-16))
	[Q R] = hQR(Ao);
	An = R*Q(1:s2,1:s2);
	errorVal = 0;
	for i=1:s1
		for j=1:s2
			if (abs(An(i,j)) < 1e-16)
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
	Ao = An;
end 	

y = An;

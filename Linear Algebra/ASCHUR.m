function [y Qout] = ASCHUR(A)
[A QHess] = HESS(A);
s = size(A,1);
Q = eye(s);
Qout = 0*Q;
Qc = eye(s);
Ao = A;
An = 0*Ao;
spda = 1;
pda(1) = 1;
epsilon = 1e-15;
errorVal = norm(An-Ao);
errorValp = 0;
errorDiff = errorVal - errorValp;
k = 1;
while ((errorVal > epsilon) && (errorDiff > 1e-16))
%for incr=1:100
	mu2cpy = diag(Ao);
	for i=1:spda %% for all principle diagonal matrices
		start = pda(i);
		if (i+1 > spda)
			stop = s;
		else
			stop = pda(i+1) - 1;
		end

		if (start < stop)
			d = -(mu2cpy(stop) - mu2cpy(stop-1))/2;
			v = Ao(stop,stop) + d - sign(d)*sqrt(abs(d*d + Ao(stop,stop-1)*Ao(stop,stop-1))); %% wilkinson shift
			mu2cpy(start:stop) = v;
		end
	end
     	mu1 = mu2cpy;
	[Q R] = hQR(Ao - diag(mu1));
	Qc = Qc*Q;
	An = R*Q + diag(mu1);
	
	errorVal = 0;
	maxv = max(diag(An));
	for i=1:s
		for j=1:s
			if (abs(An(i,j)/maxv) < 1e-15)
				An(i,j) = 0;
			end
			if (i~=j)
				errorVal = errorVal + An(i,j)*An(i,j);
			end
		 end
	end

	errorVal  = sqrt(errorVal);
	errorDiff = abs(errorVal - errorValp);
	errorValp = errorVal;

	pda = PDM(An);
	spda = size(pda,2);
	Ao = An;
	Qout = Qc'*QHess;
	k = k+1;
end

y = An;


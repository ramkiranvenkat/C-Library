function [U B V] = biDiag(inp)
A = inp;
s1 = size(A,1);
s2 = size(A,2);
U = eye(s1);
V = eye(s2);
m = min(s1,s2);
k = 1;
while (k <= m)
	x = A(k:s1,k);
	e1 = 0*x;e1(1) = 1;
	if (x(1) > 0) 
		sgn = 1;
	else
		sgn = -1;
	end  
	v = sgn*norm(x)*e1 + x;
	if (norm(v) > 1e-32)
       		v = v/norm(v);
	end

	Q = eye(s1); 
	Q(k:s1,k:s1) = (eye(s1 - k +1) - 2*v*v');
	A = Q*A;
	U = U*Q';

	if (k < m)
		x = A(k,k+1:s2);
		e1 = 0*x;e1(1) = 1;
		if (x(1) > 0) 
			sgn = 1;
		else
			sgn = -1;
		end 
		v = sgn*norm(x)*e1 + x;
    		if (norm(v) > 1e-32)
        		v = v/norm(v);
    		end
		Q = eye(s2); 
    		Q(k+1:s2,k+1:s2) = (eye(s2 - (k+1) +1) - 2*v'*v);
		A = A*Q;
		V = Q'*V;
		
	end
	k = k + 1;
end
B = A;

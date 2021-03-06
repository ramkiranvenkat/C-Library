function [A q] = HESS(inp)
A = inp;
m =size(A,2);
for k=1:m-2
	x = A(k+1:m,k);
	e1 = 0*x;
	e1(1) = 1;
	if (sign(x(1)) > 0) 
		sgn = 1;
	else
		sgn = -1;
	end  
	v = sgn*norm(x)*e1 + x;
    if (norm(v) > 1e-16)
        v = v/norm(v);
    end

    Q(:,:,k) = eye(size(A,1)); 
    Q(k+1:m,k+1:m,k) = (eye(m-(k+1)+1) - 2*v*v');

	A(k+1:m,k:m) = A(k+1:m,k:m) - 2*v*(v'*A(k+1:m,k:m));
	A(:,k+1:m)   = A(:,k+1:m)   - 2*(A(:,k+1:m)*v)*v';
end

q = eye(m);
for k=m-2:-1:1
	q = q*Q(:,:,k);
end

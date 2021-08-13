function [l q] = hLQ(inp)
A = inp;
m = min(size(A,1),size(A,2));
q = eye(size(A,2));
for k=1:m
	x = A(k,k:size(A,2));
	e1 = 0*x;e1(1) = 1;
	% v(:,k) = sign(x(1))*norm(x)*e1 + x;
	% v(:,k) = v(:,k)/norm(v(:,k));

	% A(k:size(A,1),k:size(A,2)) = A(k:size(A,1),k:size(A,2)) - 2*v(:,k)*(v(:,k)'*A(k:size(A,1),k:size(A,2)));

	if (sign(x(1)) > 0) 
		sgn = 1;
	else
		sgn = -1;
	end  
	v = sgn*norm(x)*e1 + x;
    if (norm(v) > 1e-32)
        v = v/norm(v);
    end
    Q(:,:,k) = eye(size(A,2)); 
    Q(k:size(A,2),k:size(A,2),k) = (eye(size(A,2) - k +1) - 2*v'*v);
    
	
	% A(k:size(A,1),k:size(A,2)) = A(k:size(A,1),k:size(A,2)) - 2*v*(v'*A(k:size(A,1),k:size(A,2)));
	A = A*Q(:,:,k);
end
l = A;

Corr = eye(size(A,2));
for  i=1:m;
	if (l(i,i) >= 0)
		Corr(i,i) = 1;
	else
		Corr(i,i) =  -1;
	end
end
q = Corr*eye(size(A,2));
l = l*Corr;

for k=1:m
	q = q*Q(:,:,m - k + 1);
end

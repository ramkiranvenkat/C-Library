function [q r] = hQR(A)

for k=1:min(size(A,1),size(A,2))
	x = A(k:size(A,1),k);
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

    Q(:,:,k) = eye(size(A,1)); 
    Q(k:size(A,1),k:size(A,1),k) = (eye(size(A,1) - k +1) - 2*v*v');
    
	
	% A(k:size(A,1),k:size(A,2)) = A(k:size(A,1),k:size(A,2)) - 2*v*(v'*A(k:size(A,1),k:size(A,2)));
	A = Q(:,:,k)*A;
end

r = A;
Corr = eye(size(A,1));
for  i=1:min(size(A,1),size(A,2));
	if (r(i,i) >= 0)
		Corr(i,i) = 1;
	else
		Corr(i,i) =  -1;
	end
end
q = eye(size(A,1))*Corr;
r = Corr*r;
for k=1:min(size(A,1),size(A,2))
	q = Q(:,:,min(size(A,1),size(A,2)) - k + 1)'*q;
end


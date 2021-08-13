function [P L U] = pLU(A)
m = size(A,2);
U = A;
L = eye(m);
P = eye(size(A,1));

for k=1:m-1
	[v idx] = max(abs(U(k:m,k)));
	i = k-1+idx;

	temp = U(i,k:m);
	U(i,k:m) = U(k,k:m);
	U(k,k:m) = temp;

	temp = L(i,1:k-1);
	L(i,1:k-1) = L(k,1:k-1);
	L(k,1:k-1) = temp;

	temp = P(i,:);
	P(i,:) = P(k,:);
	P(k,:) = temp;
	for j=k+1:m
		L(j,k) = U(j,k)/U(k,k);
		U(j,k:m) = U(j,k:m) - L(j,k)*U(k,k:m);
	end
end

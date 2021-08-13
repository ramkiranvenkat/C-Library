function [q r] = gQR(A)
q = 1;
for k=1:size(A,2)

	for l=k+1:size(A,1)
		the = atan2(A(l,k),A(k,k));
		J = [cos(the) sin(the);-sin(the) cos(the)];
		out = J*[A(k,:);A(l,:)];
		Q = eye(size(A,2));
		Q(k,k) = J(1,1);
		Q(k,l) = J(1,2);
		Q(l,k) = J(2,1);
		Q(l,l) = J(2,2);
		q = q*Q';
		A(k,:) = out(1,:);
		A(l,:) = out(2,:);
	end

end
r = A;
for  i=1:size(A,2);
	if (r(i,i) >= 0)
		Corr(i,i) = 1;
	else
		Corr(i,i) =  -1;
	end
end
q = q*Corr;
r = Corr*r;

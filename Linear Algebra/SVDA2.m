function [U S V] = SVDA2(inp)
A = inp;
[U1 B V1] = biDiag(A);

H = [0*B'*B B';B 0*B*B'];
[Ha Qh] = ASCHUR(H);

%%% Qh'*Ha*Qh = H
Ha = diag(diag(Ha));
[Ho Qo] = reOrder(Ha);

%%% Qo*Ho*Qo' = Ha
%%% Qh'*Qo*Ho*Qo'*Qh = H
Q = Qo'*Qh;
Qc = Q';
for i=1:size(H,1)/2

	intVal = abs(sum(Qc(1:size(H,1)/2,i) - Qc(1:size(H,1)/2,i+size(H,1)/2)));
	if (intVal > 2e-15)
		M = eye(size(H,1));
		M(i,i) = -1;
		Qc = Qc*M;
	end
end

Vnew = sqrt(2)*Qc(1:size(A,2),1:size(A,2));
Unew = sqrt(2)*Qc((size(A,2)+1):size(H,2),(size(A,2)+1):size(H,2));

S = Ho(1:size(H,1)/2,1:size(H,1)/2);

if (norm(Unew*S*Vnew' - B) > 2e-15)
	Unew = - Unew;
end

U = U1*Unew;
V = Vnew'*V1;
V = V';

function [U S V] = SVDA3(inp)
U = 0;V = 0;S = 0;
A = inp;
[U1 B V1] = biDiag(A);
[Ba Qb] = ASCHUR(B);
U = U1*Qb';
S = Ba;
V = (Qb*V1)'

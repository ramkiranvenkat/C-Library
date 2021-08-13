#include "isnum.h"
NUM numok(STRING istr)
{       NUM check = isnum(istr);
        if (check.t != 'c')
        return check;
        int itr,ev;
        STRING base = malloc((strlen(istr)+1)*sizeof(char));
        for (itr = 0;itr<strlen(istr);itr++)
        {       if ((istr[itr] == 'e')||(istr[itr] == 'E'))
                        break;
                base[itr] = istr[itr];         
        }
        ev = itr;
        if ((ev == 0)||((strlen(istr)-ev) == 1)) return check;
        base[ev] = '\0';
        base = realloc(base,(ev+1)*sizeof(char));
        
        STRING power = malloc((strlen(istr)-ev)*sizeof(char));
        for (itr = ev+1;itr<strlen(istr)+1;itr++)
        power[itr-ev-1] = istr[itr];   
        
        NUM bs = isnum(base);
        NUM pw = isnum(power);

        if ((bs.t == 'c')||(pw.t == 'c'))
                return check;

        NUM out;
        out.d = bs.d*pow(10.0,pw.d);
        out.l = (long)out.d;
        out.i = (int)out.d;
        out.f = (float)out.d;
        out.t = 'e';

	free(base);
	free(power);
        return out;
        
}
NUM isnum(STRING argc)
{
        STRING inp;

        NUM out;
        out.i = 0;
        out.l = 0;
        out.f = 0;
        out.d = 0;
        out.t = 'c';

        inp = malloc(strlen(argc)*sizeof(char));

        if (inp == NULL)
        {       printf("Memory not avalable\n");
                out.t = '\0';
                return out;
        }
        strcpy(inp,argc);
        
        ret sign = checksgn(inp);
        
        if (sign.s)
        {       int itr;
                STRING numstr = malloc((strlen(argc)-sign.c+1)*sizeof(char));
                
                for (itr = 0;itr < (strlen(argc)-sign.c);itr++)
                numstr[itr] = inp[itr + sign.c];
                numstr[strlen(argc)-sign.c] = '\0';
                ret numflg = checkint(numstr);

                if ((!numflg.s)&&!(strlen(numstr)-numflg.c)) 
                {       out.l = sign.s*atol(numstr);
                        out.i = sign.s*atoi(numstr);
                        out.f = sign.s*atof(numstr);
                        out.d = sign.s*atof(numstr);
                        out.t = 'i';
 
			free(numstr);free(inp);
                        return out;    
                }
                else if ((!numflg.s)&&(strlen(numstr)-numflg.c))
                {
                        STRING dcmstr = malloc((strlen(numstr)-numflg.c)*sizeof(char));
                        for (itr = numflg.c+1;itr<strlen(numstr)+1;itr++)
                        dcmstr[itr-numflg.c-1] = numstr[itr];
                        ret dcmflg = checkint(dcmstr);

                        if ((!dcmflg.s)&&!(strlen(dcmstr)-dcmflg.c)) 
                        {       out.l = sign.s*atol(numstr);
                                out.i = sign.s*atoi(numstr);
                                out.f = sign.s*atof(numstr);
                                out.d = sign.s*atof(numstr);
                                out.t = 'f';
				
				free(dcmstr);free(numstr);free(inp);
                                return out;
                        }
                        else 
			{
				free(dcmstr);free(numstr);free(inp);
				return out;
			}
                }
                else 
		{	
			free(numstr);free(inp);
			return out;
		}

        }
        else 
	{	
		free(inp);
		return out;
	}
}

ret checkint(STRING inp)
{       int l = strlen(inp),flag = 0,i = 0;
        ret out;
        while ((i<l)&&!(inp[i] == '.')&&(!flag))
        {       
                if (!((inp[i] >= '0') && (inp[i] <= '9')))
                {       flag++;
                        break;
                }
                i++;
        }

        out.s = flag;
        out.c = i;
        return out;
        
}

ret checksgn(STRING inp)
{       int l = strlen(inp),i = 0,count = 1;
        ret out;
        while((i<l)&&(count))
        {       
                if (!((inp[i] == '+')||(inp[i] == '-')))
                break;

                if (inp[i] == '+')
                {count *=  1;}
                else if (inp[i] == '-')
                {count *= -1;}
                else 
                {count = 0;}
                i++;                
        }
        
        out.c = i;
        out.s = count;
        return out;
        
}

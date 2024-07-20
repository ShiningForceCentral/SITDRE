

#include <idc.idc>


static main(void){
    
    Message("PARSING DATA STRUCTURES INTO MACROS ...\n");
    parseNames(0x69E4,0x6DDB);
    parseNames(0x6DDB,0x6FF1);
    parseNames(0x6FF1,0x74B2);
    parseNames(0x74B2,0x7527);
    parseNames(0x7527,0x755B);
    parseNames(0x755B,0x75F9);
    
    Message("END OF PARSING.\n");  
}


static parseNames(start,end){
    auto addr,len,name,j;
    addr = start;
    while(addr<end){
        len = Byte(addr);
        name = str(addr+1,len-1);
        for(j=addr;j<addr+len;j++){undefineByte(j);}
        MakeData(addr,FF_BYTE,len,1);
        SetManualInsn(addr, form("str '%s'",name));
        addr = addr+len;
    }
}


/*
 *  Return a length byte prefixed string.
 */
static str(addr,len){
    auto str,j,chr;
    
    str = "";
    for(j=addr;j<addr+len;j++){
        chr = Byte(j);
        
        /* Convert character to ASCII code */
        if(chr==0){
            chr = 32;       // Space
        }else if(chr>=1 && chr<=10){
            chr = chr+47;   // 0-9
        }else if(chr>=11 && chr<=36){
            chr = chr+86;   // a-z
        }else if(chr>=37 && chr<=62){
            chr = chr+28;   // A-Z
        }else if(chr==63){
            chr = 39;       // '
        }else if(chr==64){
            chr = 44;       // ,
        }else if(chr==65){
            chr = 33;       // !
        }else if(chr==66){
            chr = 63;       // ?
        }else if(chr==67){
            chr = 45;       // -
        }else if(chr==68){
            chr = 149;      // •
        }else if(chr==69){
            chr = 47;       // /
        }else if(chr==70){
            chr = 46;       // .
        }else if(chr==71){
            chr = 40;       // (
        }else if(chr==72){
            chr = 41;       // )
        }else if(chr==73){
            chr = 34;       // "
        }else if(chr==74){
            chr = 58;       // :
        }else if(chr==75){
            chr = 35;       // #
        }
        
        str = form("%s%c",str,chr);
    }
    
    return str;
}


/* 
 *  Makes sure byte at addr is clean for new formatting.
 */
static undefineByte(addr){     
    MakeUnkn(addr,DOUNK_DELNAMES);
    SetManualInsn(addr,"");
}


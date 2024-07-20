

#include <idc.idc>


static main(void){
    splitAll();
}


static splitAll(){
    auto file;
    
    Message("SPLITTING...\n");
    file = fopen("sitdsplits.txt","w");
    initFile(file);
    
    Message("Single Chunks...");    
    splitSingleChunks(file);    
    Message(" DONE.\n");    
    
    writestr(file,"\nsplit    0x0,0x0,\\0x0 - .bin\n");
    fclose(file);
    Message("END OF SPLITS.\n");    
}


static initFile(file){
    writestr(file,"/***********************Directories***************************/\n");
    writestr(file,"#dir    data/\n");
    writestr(file,"#dir    data/sound/\n");
    
    writestr(file,"\n");
    writestr(file,"/***********************Data***************************/\n");
    writestr(file,"\n"); 
}


static splitSingleChunks(file) {
    auto i,j,x,s,index,path;
    auto start,base,addr,end,dref,section,action;
    
    // Template : splitSingleChunk(0x,0x,"","",file);
    
    MakeAlign(0x75F9, 0x8000-0x75F9,0);
    MakeAlign(0xFFE6, 0x10000-0xFFE6,0);
    MakeAlign(0x1761E, 0x18000-0x1761E,0);
    MakeAlign(0x23DF5, 0x24000-0x23DF5,0);
    MakeAlign(0x3FE1F, 0x40000-0x3FE1F,0);
    MakeAlign(0x4FC18, 0x50000-0x4FC18,0);
    MakeAlign(0x5747C, 0x58000-0x5747C,0);
    MakeAlign(0x7FFD8, 0x80000-0x7FFD8,0);
    MakeAlign(0x9BECA, 0x9C000-0x9BECA,0);
    MakeAlign(0x9FCC2, 0xA0000-0x9FCC2,0);
    MakeAlign(0xB7DC0, 0xB8000-0xB7DC0,0);
    MakeAlign(0xBFF1E, 0xC0000-0xBFF1E,0);
    MakeAlign(0xDBFD2, 0xDC000-0xDBFD2,0);
    MakeAlign(0xDFF93, 0xE0000-0xDFF93,0);
    MakeAlign(0xE3AFB, 0xE4000-0xE3AFB,0);
    
    splitSingleChunkWithCommentedSplitEntry(0xF191C,0xF341C,"SoundDriver","data/sound/sounddriver.bin",file);
    MakeAlign(0xF341C, 0xF4000-0xF341C,0);
    MakeAlign(0xF7FC4, 0xF8000-0xF7FC4,0);
    MakeAlign(0xFFF2F, 0x100000-0xFFF2F,0);
}


static splitSingleChunk(start, end, nameString, binPath, splitFile){
    splitSingleChunkWithConditionalIncbin(start, end, nameString, binPath, splitFile, 0);
}


static splitSingleChunkWithConditionalIncbin(start, end, nameString, binPath, splitFile, incbinType){
    auto j,incbin;
    
    //Message("Cleaning from %a to %a ...\n",start,end);
    for(j=start+1;j<end;j++){undefineByte(j);}
    MakeData(start,FF_BYTE,end-start,1);
    if(nameString!=""){
        MakeNameEx(start,nameString,0);
    }
    if(incbinType==1){
        incbin = "incbinIfVanillaRom";
    }else{
        incbin = "incbin";
    }
    SetManualInsn   (start, form("%s \"%s\"",incbin,binPath));
    writestr(splitFile,form("#split\t0x%s,0x%s,%s\n",ltoa(start,16),ltoa(end,16),binPath));
    //Jump(start);
}


static splitSingleChunkWithCommentedSplitEntry(start, end, nameString, binPath, splitFile){
    auto j;
    
    //Message("Cleaning from %a to %a ...\n",start,end);
    for(j=start+1;j<end;j++){undefineByte(j);}
    MakeData(start,FF_BYTE,end-start,1);
    MakeNameEx(start,nameString,0);
    SetManualInsn   (start, form("incbin \"%s\"",binPath));
    writestr(splitFile,form("/*#split\t0x%s,0x%s,%s*/\n",ltoa(start,16),ltoa(end,16),binPath));
    //Jump(start);
}


/* 
 *  Makes sure byte at addr is
 *  clean for new formatting
 */
static undefineByte(addr){
    auto from;
    
    from = DfirstB(addr);
    while(from!=BADADDR){
        //Message(form("Removed DATA XRef at addr %a, from %a\n",addr, from));
        del_dref(from,addr);
        from=DnextB(addr,from);
    }
    
    from = RfirstB(addr);
    while(from!=BADADDR){
        //Message(form("Removed Code XRrf at addr %a, from %a\n",addr, from));
        DelCodeXref(from,addr,1);
        from=RnextB(addr,from);
    }
    
    MakeUnkn(addr,DOUNK_DELNAMES);
    MakeNameEx(addr,"",0);
    SetManualInsn(addr,"");
}






#include <idc.idc>

static main(void) {

    Message("PRODUCING ASM...\n");

    //produceMacros();
    produceEnums();
    produceConst();
    produceMain();
    
    Message("\nEND OF ASM PRODUCTION.\n");

}

static produceMacros(){
}

static produceEnums(){
    auto i,j,enumQty,id,enumName,bmask,enumCount,constant,constId,constCmt,output,file;
    file = fopen("disasm\\sitdenums.asm","w");
    Message("\nWriting Enums to sitdenums.asm ...");
    enumQty = GetEnumQty();
    for(i=0;i<enumQty;i++){
        id = GetnEnum(i);
        enumName = GetEnumName(id);
        writestr(file,"\n\n; ---------------------------------------------------------------------------\n");
        bmask = GetFirstBmask(id);
        if(bmask==-1){writestr(file,form("\n; enum %s",enumName));}
        else{writestr(file,form("\n; enum %s (bitfield)",enumName));}
        
        /* Iterate bitmasks */
        while(bmask!=0){
            enumCount = GetEnumSize(id);
            constant = GetFirstConst(id,bmask);
            
            /* Iterate constants */
            while(enumCount>0){
                j=0;
                constId = GetConstEx(id,constant,j,bmask);
                while(constId != -1){
                    output = conditionalEnumOutput(id,enumName,enumCount,constant,constId);
                    writestr(file,form("\n%s", output));
                    j++;
                    constId = GetConstEx(id,constant,j,bmask);
                }
                constant = GetNextConst(id,constant,bmask);
                enumCount--;
            }
            
            /* Break out of loop after iterating last bitmask */
            if(bmask==-1){break;}
            bmask = GetNextBmask(id,bmask);
        }
    }
    fclose(file);
    Message("DONE.");
}

static conditionalEnumOutput(id,enumName,enumCount,constant,constId){
    auto i,j,constName,constCmt,enumMember,enumFlags,output;
    constName = GetConstName(constId);
    
    /* Format comment string */
    constCmt = GetConstCmt(constId,0);
    if(constCmt==""){
        constCmt = GetConstCmt(constId,1);
        if(constCmt!=""){
            constCmt = formatRptCmt(constCmt);
        }
    }
    if(constCmt!="" && constCmt!=" "){
        constCmt = form(" ; %s",constCmt);
    }
    //Message(form("constId(%d): \"%s\"\n", constId, constCmt));
    
    /* Format enum member string using the proper numerical representation */
    enumFlags = GetEnumFlag(id);
    if(constant>=0 && constant<10 || (enumFlags&FF_1NUMD)!=0){
        output = form("%s", formEnumMember(constName,constant,constCmt));
    }
    else if((enumFlags&FF_1NUMH)!=0){
        output = form("%s", formEnumMemberWithHex(constName,constant,constCmt));
    }
    else{
        output = form("%s", formEnumMemberWithBin(constName,constant,constCmt));
    }
    //Message(form("output: \"%s\"\n", output));
    
    return output;
    
}

static formEnumMember(constName,constant,constCmt) {
    auto enumMember;
    enumMember = formEnumMemberEx(constName,constant,constCmt,0);
    return enumMember;
}

static formEnumMemberWithHex(constName,constant,constCmt) {
    auto enumMember;
    enumMember = formEnumMemberEx(constName,constant,constCmt,1);
    return enumMember;
}

static formEnumMemberWithBin(constName,constant,constCmt) {
    auto enumMember;
    enumMember = formEnumMemberEx(constName,constant,constCmt,-1);
    return enumMember;
}

static formEnumMemberEx(constName,constant,constCmt,rep) {
    auto operand,constStr,enumMember;
    if(rep==0){
        operand = "";
        constStr = ltoa(constant,10);
    }
    else if(rep==1){
        operand = "$";
        constStr = ltoa(constant,16);
    }
    else{
        operand = "%";
        constStr = ltoa(constant,2);
    }
    enumMember = form("%s: equ %s%s%s", constName, operand, constStr, constCmt);
    //Message(form("enumMember: \"%s\"\n", enumMember));
    
    return enumMember;
}


static produceConst(void) {
    auto seg,end,ea,segName,name,comment,commentEx,file;
    Message("\nWriting offset constants to sitdconst.asm ...");
    file = fopen("disasm\\sitdconst.asm","w");
    writestr(file,"; SITDCONST.ASM INCLUDED AT START OF SITD.ASM\n\n");
    seg = FirstSeg();
    seg = NextSeg(seg);
    while(seg != BADADDR){
        end = SegEnd(seg);
        segName = SegName(seg);
        writestr(file,form("; START OF SEGMENT %s OFFSETS FROM %a TO %a\n",segName, seg, end));
        ea = seg;
        while(ea<end){
            name = GetTrueNameEx(ea,ea);
            if(name==""){
                name = Name(ea);
            }
            comment = CommentEx(ea,0);
            if(comment==""){
                comment = CommentEx(ea,1);
                if(comment!=""){
                    comment = formatRptCmt(comment);
                }
            }
            if(comment!="" && comment!=" "){
                comment = form(" ; %s",comment);
            }
            if(name!=""){
                writestr(file,form("%s: equ $%s%s\n",name,ltoa(ea,16),comment));
            }
            ea = ea + ItemSize(ea);
        }
        writestr(file,form("; END OF SEGMENT %s OFFSETS FROM %a TO %a\n\n",segName, seg, end));
        seg = NextSeg(seg);
    }
    fclose(file);
    Message("DONE.\n");
}


static produceMain(){
    auto action, file;
    
    Message("Writing main assembly file to sitd.asm ...");
    action = 1;
    file = fopen("disasm\\sitd.asm","w");
    writeHeader(file);
    
    produceSection(file,"01",   0x0,        0x8000,     0x8000-0x75F9,      "");
    produceSection(file,"02",   0x8000,     0x10000,    0x10000-0xFFE6,     "");
    produceSection(file,"03",   0x10000,    0x18000,    0x18000-0x1761E,    "");
    produceSection(file,"04",   0x18000,    0x24000,    0x24000-0x23DF5,    "");
    produceSection(file,"05",   0x24000,    0x40000,    0x40000-0x3FE1F,    "");
    produceSection(file,"06",   0x40000,    0x50000,    0x50000-0x4FC18,    "");
    produceSection(file,"07",   0x50000,    0x58000,    0x58000-0x5747C,    "");
    produceSection(file,"08",   0x58000,    0x80000,    0x80000-0x7FFD8,    "");
    produceSection(file,"09",   0x80000,    0x9C000,    0x9C000-0x9BECA,    "");
    produceSection(file,"10",   0x9C000,    0xA0000,    0xA0000-0x9FCC2,    "");
    produceSection(file,"11",   0xA0000,    0xB8000,    0xB8000-0xB7DC0,    "");
    produceSection(file,"12",   0xB8000,    0xC0000,    0xC0000-0xBFF1E,    "");
    produceSection(file,"13",   0xC0000,    0xDC000,    0xDC000-0xDBFD2,    "");
    produceSection(file,"14",   0xDC000,    0xE0000,    0xE0000-0xDFF93,    "");
    produceSection(file,"15",   0xE0000,    0xE4000,    0xE4000-0xE3AFB,    "");
    produceSection(file,"16",   0xE4000,    0xF4000,    0xF4000-0xF341C,    "");
    produceSection(file,"17",   0xF4000,    0xF8000,    0xF8000-0xF7FC4,    "");
    produceSection(file,"18",   0xF8000,    0x100000,   0x100000-0xFFF2F,   "");
    
    fclose(file);
    Message("\nDONE.");   
}


static produceSection(mainFile,sectionName,start,end,fs,sectionComment){
    auto fileName;
    fileName = form("sitd-%s.asm",sectionName);
    produceSectionWithPrettyPrintParam(mainFile,sectionName,start,end,fs,sectionComment,1,fileName);
}

static produceSectionWithPrettyPrintParam(mainFile,sectionName,start,end,fs,sectionComment,prettyPrint,fileName){
    auto file,ea,itemSize;
    
    Message(form("Writing assembly section %s to %s (%s) ... ",sectionName,fileName,sectionComment));
    writestr(mainFile,form("\t\tinclude \"%s\"\t\t; %s\n",fileName,sectionComment));
    file = fopen(form("disasm\\%s",fileName),"w");
    writestr(file,form("\n; GAME SECTION %s :\n; %s\n",sectionName,sectionComment));
    writestr(file,form("; FREE SPACE : %d bytes.\n\n\n",fs));
    
    ea = start;
    while(ea<end){
        
        itemSize = ItemSize(ea);
        if(GetFunctionAttr(ea,FUNCATTR_START)==ea){    
            writeFunctionHeader(file,ea,prettyPrint);
        }
        else if(GetFchunkAttr(ea,FUNCATTR_START)==ea){    
            writeFChunkHeader(file,ea,prettyPrint);
        }        
        writeItem(file,ea);
        if(GetFunctionAttr(ea,FUNCATTR_END)==(ea+itemSize)){
            writeFunctionFooter(file,ea,prettyPrint);
        }        
        else if(GetFchunkAttr(ea,FUNCATTR_END)==(ea+itemSize)){
            writeFChunkFooter(file,ea,prettyPrint);
        }
        ea = ea+itemSize;
        
    }
    
    fclose(file);
    Message("DONE.\n");
}


static writeHeader(file){
    writestr(file,"\n");
    writestr(file,"   include \"sitdmacros.asm\"\n");    
    writestr(file,"   include \"sitdenums.asm\"\n");
    writestr(file,"   include \"sitdconst.asm\"\n");
    writestr(file,"\n");
}

static writeFooter(file){
    writestr(file,"        END");
}


static writeFunctionHeader(file, ea, prettyWriteFunctions){
    auto funcCmt;
    
    if(prettyWriteFunctions!=0){
        writestr(file,"\n; =============== S U B R O U T I N E =======================================\n\n");
    }
    
    funcCmt = GetFunctionCmt(ea,0);
    if(funcCmt!=""){
        writestr(file,form("; %s\n",funcCmt));
        if(prettyWriteFunctions!=0){
            writestr(file,"\n");
        }
    }
    else{
        funcCmt = GetFunctionCmt(ea,1);
        if(funcCmt!=""){
            funcCmt = formatFuncRptCmt(funcCmt);
            writestr(file,form("; %s\n",funcCmt));
            if(prettyWriteFunctions!=0){
                writestr(file,"\n");
            }            
        }
    }
    undefineLocalVars(file,ea);
    //writeLocalVariables(file,ea);
}

static undefineLocalVars(file,ea){
    auto id, i, firstM, lastM, mName, mSize, mFlag;
    id = GetFrame(ea);
    firstM = GetFirstMember(id);
    lastM = GetLastMember(id);
    i=firstM;
    while(i<=lastM){
        if(i!=-1&&GetMemberName(id,i)!=""&&GetMemberName(id,i)!=" r"&&GetMemberName(id,i)!=" s"){
            mName = GetMemberName(id,i); // Get the name
            mSize = GetMemberSize(id, i); // Get the size (in byte)
            mFlag = GetMemberFlag(id, i); // Get the flag
            Message("\n%a : undefined %s %d %x", ea, mName, mSize, mFlag);
            DelStrucMember(id,i);        
            i = i+mSize;
        }
        else{
            i++;
        }    
    }
}


static undefineMultipleLineArray(ea){
    auto type;
            type = GuessType(ea);
            if(strstr(type,"char[")!=-1&&(strstr(GetDisasm(ea),"incbin")==-1)){
                    Message(form("\n%a : %s \t\t<== UNDEFINE if on multiple lines.",ea,GuessType(ea)));
            }    
}

static writeFunctionFooter(file, ea,prettyWriteFunctions){
    auto funcName;
    funcName = GetFunctionName(ea);
    if(prettyWriteFunctions!=0){
        writestr(file,form("\n    ; End of function %s\n\n",funcName));
    }
}


static writeItem(file,ea){
    writeItemWithPrettyPrintParam(file,ea,1);
}

static writeItemWithPrettyPrintParam(file,ea,prettyPrint){
    auto name,tabLength,ln,indent,disasm,cmtIdx,commentIndent,comment,commentEx,i,line,lineA,lineB,type,output;
    tabLength = 8;
    indent = "\t\t";
    commentIndent = "\t\t\t\t";
    name = GetTrueNameEx(ea,ea);        // Get local label name if it exists
    if(name==""){
        name = Name(ea);
    }
    if(name!=""){
        name=form("%s:",name);
        if(GetFunctionAttr(ea,FUNCATTR_START)!=-1){
            if(prettyPrint!=0){
                ln = "\n";
            }else{
                ln = "";
            }
            name = form("%s%s%s",name,ln,indent);
        }
        else{
            while(strlen(name)<(strlen(indent)*tabLength)){
                name = form("%s ",name);
            }        
            undefineMultipleLineArray(ea);
        }
    }
    else {
        name = indent;
    }
    commentEx = CommentEx(ea,0);
    if(commentEx!=""){
        comment = commentEx;
    }
    else{
        commentEx = CommentEx(ea,1);
        if(commentEx!=""){
            comment = formatRptCmt(commentEx);
        }
    }
    lineA = LineA(ea,0);
    disasm = GetDisasm(ea);
    cmtIdx = strstr(disasm,";");
    lineB = LineB(ea,0);
    if(lineA!=""){
        lineA = form("%s\n",lineA);
    }
    if(lineB!=""){
        lineB = form("%s\n",lineB);
    }    
    if(cmtIdx!=-1){
        disasm = substr(disasm,0,cmtIdx);
    }
    if(comment!=""){
        comment = form("; %s",comment);
    }
    if(strlen(name)>(strlen(indent)*tabLength)){
        name = form("%s\n%s",name,indent);
    }
    if(strstr(lineA,"\n")!=-1){
        lineA = form("%s%s",lineA,indent);
    }
    if(strlen(disasm)>(strlen(commentIndent)*tabLength)&&comment!=""){
        disasm = form("%s\n%s%s",disasm,indent,commentIndent);
    }
    //Message(form("\nname=%s,lineA=%s,disasm=%s,comment=%s,lineB=%s",name,lineA,disasm,comment,lineB));        
    output = form("%s%s%s%s\n%s",name,lineA,disasm,comment,lineB);
    writestr(file,output);
}


static writeFChunkHeader(file,ea,prettyWriteFunctions){
    auto text,functionName;
    text = "; START OF FUNCTION CHUNK FOR ";
    functionName = GetFunctionName(ea);
    writestr(file,form("\n%s%s\n\n",text,functionName));
}

static writeFChunkFooter(file,ea,prettyWriteFunctions){
    auto text,functionName;
    text = "; END OF FUNCTION CHUNK FOR ";
    functionName = GetFunctionName(ea);
    writestr(file,form("\n%s%s\n\n",text,functionName));
}

static formatFuncRptCmt(cmt){
    auto index, before, after, result;
    index = strstr(cmt,"\n");
    if(index!=-1){
        before = substr(cmt,0,index+1);
        after = substr(cmt,index+1,strlen(cmt));
        result = form("%s; %s",before,formatFuncRptCmt(after));
        return result;
    }
    else{
        return cmt;
    }
}

static formatRptCmt(cmt){
    auto index, before, after, result;
    index = strstr(cmt,"\n");
    if(index!=-1){
        before = substr(cmt,0,index+1);
        after = substr(cmt,index+1,strlen(cmt));
        result = form("%s\t\t\t\t\t\t\t\t\t\t\t\t\t\t; %s",before,formatRptCmt(after));
        return result;
    }
    else{
        return cmt;
    }
}

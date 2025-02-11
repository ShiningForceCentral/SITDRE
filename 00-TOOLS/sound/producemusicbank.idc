
/*
 *  Attempt at producing ASM file from IDC script 
 *  instead of IDA's function, for more flexibility.
 */

#include <idc.idc>

static main(void) {

    Message("PRODUCING MUSIC BANK 0...\n");

    produceMain();
    
    Message("\nEND OF ASM PRODUCTION.\n");

}


static produceMain(){

    auto seg,end,ea,itemSize,action,currentLine,previousLine,file;
    auto output, name, indent, comment, commentEx, commentIndent;
    Message("Writing main assembly file to musicbank.asm ...");    
    action = 1;
    file = fopen("musicbank.asm","w");
    writeHeader(file);

    produceMusicBank(file);

    fclose(file);
    Message("\nDONE.");    
}

static produceMusicBank(mainFile){

    produceAsmSection(mainFile,0x8900,0x8940);
    produceAsmScriptWithConditionalInclude(mainFile,"music01",0x8940,0x8B1A,"Music 01",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music02",0x8B1A,0x92A9,"Music 02",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music04",0x92A9,0x94A8,"Music 04",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music05",0x94A8,0x9E7F,"Music 05",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music06",0x9E7F,0xAF67,"Music 06",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music07",0xAF67,0xB55A,"Music 07",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music08",0xB55A,0xB856,"Music 08",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music09",0xB856,0xC076,"Music 09",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music19",0xC076,0xC2B2,"Music 19",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music24",0xC2B2,0xC8BB,"Music 24",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music13",0xC8BB,0xC96F,"Music 13",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music15",0xC96F,0xCC0F,"Music 15",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music18",0xCC0F,0xCD2B,"Music 18",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music25",0xCD2B,0xCDDC,"Music 25",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music27",0xCDDC,0xCEE2,"Music 27",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music03",0xCEE2,0xD471,"Music 03",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music11",0xD471,0xE033,"Music 11",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music20",0xE033,0xE2AB,"Music 20",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music21",0xE2AB,0xE50E,"Music 21",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music10",0xE50E,0xE78D,"Music 10",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music17",0xE78D,0xE828,"Music 17",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music22",0xE828,0xE948,"Music 22",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music16",0xE948,0xEA77,"Music 16",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music23",0xEA77,0xEB2A,"Music 23",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music31",0xEB2A,0xEE4C,"Music 31",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music30",0xEE4C,0xEF35,"Music 30",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music28",0xEF35,0xEFE9,"Music 28",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music29",0xEFE9,0xFF14,"Music 29",0);
    produceAsmScriptWithConditionalInclude(mainFile,"music32",0xFF14,0xFF2F,"Music 32",0);
    
}


static produceSection(mainFile,sectionName,start,end,fs,sectionComment){
    produceSectionWithPrettyPrintParam(mainFile,sectionName,start,end,fs,sectionComment,1);
}


static produceSectionWithPrettyPrintParam(mainFile,sectionName,start,end,fs,sectionComment,prettyPrint){
    auto ea,itemSize,action,currentLine,previousLine,fileName,file;
    auto output, name, indent, comment, commentEx, commentIndent;
    fileName = form("layout\\sf2-%s-0x%s-0x%s.asm",sectionName,ltoa(start,16),ltoa(end,16));
    Message(form("Writing assembly section %s to %s (%s) ... ",sectionName,fileName,sectionComment));    
    //form("0x%s..0x%s %s",ltoa(start,16),ltoa(end,16),sectionComment)
    action = 1;
    writestr(mainFile,form("\t\tinclude \"%s\"\t\t; %s\n",fileName,sectionComment));
    file = fopen(form("%s",fileName),"w");
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
        ea = ea + itemSize;
    }
    fclose(file);
    Message("DONE.\n");    

}

static produceAsmScript(mainFile,sectionName,start,end,sectionComment){
    
    produceAsmScriptWithConditionalInclude(mainFile,sectionName,start,end,sectionComment,0);

}


static produceAsmScriptWithConditionalInclude(mainFile,sectionName,start,end,sectionComment,conditionalIncludeType){
    auto ea,itemSize,action,currentLine,previousLine,fileName,file;
    auto output, include, name, indent, comment, commentEx, commentIndent, offsets;
    fileName = form("%s.asm",sectionName);
    Message(form("Writing assembly script section %s to %s (%s) ... ",sectionName,fileName,sectionComment));    
    //form("0x%s..0x%s %s",ltoa(start,16),ltoa(end,16),sectionComment)
    action = 1;
    if(conditionalIncludeType==1){
        include = "includeIfVanillaRom";
    }else if(conditionalIncludeType==2){
        include = "includeIfExpandedRom";
    }else{
        include = "include";
    }
    writestr(mainFile,form("\t\t%s \"%s\"\t\t; %s\n",include,fileName,sectionComment));
    file = fopen(form("%s",fileName),"w");
    offsets = form("0x%s..0x%s",ltoa(start,16),ltoa(end,16));
    writestr(file,form("\n; ASM FILE %s.asm :\n; %s : %s\n",sectionName,offsets,sectionComment));
    produceAsmSection(file,start,end);
    fclose(file);
    Message("DONE.\n");    

}

static produceAsmSection(file,start,end){
    produceAsmSectionWithPrettyParam(file,start,end,1);
}

static produceAsmSectionNoPretty(file,start,end){
    produceAsmSectionWithPrettyParam(file,start,end,0);
}

static produceAsmSectionWithPrettyParam(file,start,end,prettyWriteFunctions){
    auto ea,itemSize;
    ea = start;
    while(ea<end){
        itemSize = ItemSize(ea);
        if(GetFunctionAttr(ea,FUNCATTR_START)==ea){    
            writeFunctionHeader(file,ea,prettyWriteFunctions);
        }
        else if(GetFchunkAttr(ea,FUNCATTR_START)==ea){    
            writeFChunkHeader(file,ea,prettyWriteFunctions);
        }        
        writeItemWithPrettyPrintParam(file,ea,prettyWriteFunctions);
        if(GetFunctionAttr(ea,FUNCATTR_END)==(ea+itemSize)){
            writeFunctionFooter(file,ea,prettyWriteFunctions);
        }        
        else if(GetFchunkAttr(ea,FUNCATTR_END)==(ea+itemSize)){
            writeFChunkFooter(file,ea,prettyWriteFunctions);
        }                
        ea = ea + itemSize;
    }
}

static writeHeader(file){
    writestr(file,"\n");
    writestr(file,"   cpu z80\n");
    writestr(file,"   listing off\n");
    writestr(file,"   phase    0\n");
    writestr(file,"   include \"..\\musicmacros.asm\"\n");
    writestr(file,"   include \"..\\musicenums.asm\"\n");
    writestr(file,"   org 08900h\n");
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
    name = GetTrueName(ea);
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




    /* useless
    i=0;
    line = LineA(ea,i);
    while(line!=""){
        lineA = form("%s\n%s",lineA,line);
        i++;
        line = LineA(ea,i);
    }
    if(lineA!=""){
        lineA = form("%s\n",lineA);
    }
    
    i=0;
    line = LineB(ea,i);
    while(line!=""){
        lineA = form("%s\n%s",lineB,line);
        i++;
        line = LineB(ea,i);        
    }
    if(lineB!=""){
        lineB = form("%s\n",lineB);
    }
    */
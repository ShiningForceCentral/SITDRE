
/*
 *    FIXES SCRIPT 
 *    This script will contain all instruction for fixes
 *    on the IDB after the main IDC script has been applied.
 *
 *
 *    TABLE FIXES :
 *     IDA's IDC export function does not properly export 
 *    the following structures, when they
 *    are located after address 0xFFFF :
 *    - Relative Jump Tables (RJT, labeled with prefix "rjt_")
 *    - Branch Tables (BT, labeled with prefix "bt_")
 *    - Relative Pointer Tables (RPT, labeled with prefix "rpt_")
 *    This script contains the needed instructions to 
 *    fix all the RJTs/BTs/RPTs that are defined in the IDB. 
 *    Beware :
 *    Everytime a new RJT/BT/RPT is defined after 0xFFFF, 
 *    it also has    to be defined in this script.
 *
 *    
 *    INSTRUCTION FIXES :
 *    Some instructions are not properly represented by IDA.
 *    Details in more comments below.
 *
 */


#include <idc.idc>


static main(void){
    
    Message("FIXING...\n");
    
    Message("Relative Jump Tables...");
    fixRJTs();
    Message(" DONE.\nInstruction Representations...");            
    fixInstructionRepresentations();
    
    Message(" DONE.\n");
    Message("END OF FIXES.\n");
}


static fixRJTs(){
    makeRjt(0xFE4C,0xFE5C);
    makeRjt(0xFED2,0xFEF0);
    makeRjt(0x2010A,0x20112);
    makeRjt(0x20160,0x20168);
    makeRjt(0x201BC,0x201C2);
    makeRjt(0x20BC6,0x20BDE);
    makeRjt(0x20DD8,0x20E04);
    makeRjt(0x20E38,0x20E50);
    makeRjt(0x20F8E,0x20FA6);
    makeRjt(0x21024,0x2103C);
    makeRjt(0x212AA,0x212B4);
    makeRjt(0x21316,0x21320);
    makeRjt(0x2134E,0x21358);
    makeRjt(0x213C0,0x213CA);
    makeRjt(0x2147A,0x21492);
    makeRjt(0x21544,0x2155C);
    makeRjt(0x2160C,0x21624);
    makeRjt(0x216C2,0x216D0);
    makeRjt(0x21728,0x21736);
    makeRjt(0x21E20,0x21E38);
    makeRjt(0x21FD2,0x21FE6);
    makeRjt(0x22006,0x2201E);
    makeRjt(0x2206C,0x22084);
    makeRjt(0x22126,0x2213E);
    makeRjt(0x222CE,0x222E6);
    makeRjt(0x22472,0x2248A);
    makeRjt(0x2260C,0x22624);
    makeRjt(0x227B6,0x227C0);
    makeRjt(0x22878,0x22890);
    makeRjt(0x22A7E,0x22A8A);
    makeRjt(0x22B36,0x22B3C);
    makeRjt(0x243D0,0x243E0);
    makeRjt(0x2455E,0x2456E);
    makeRjt(0x24964,0x24974);
    makeRjt(0x26D02,0x26D12);
    makeRjt(0x26E00,0x26E0C);
    makeRjt(0x26EEE,0x26EF6);
    makeRjt(0x27800,0x2780C);
    makeRjt(0x2808C,0x28098);
    makeRjt(0x2851C,0x28522);
    makeRjt(0x28CDA,0x28CFE);
    makeRjt(0x28D24,0x28D2A);
    makeRjt(0x28D5E,0x28D66);
    makeRjt(0x29076,0x2907A);
    makeRjt(0x294A4,0x294AA);
    makeRjt(0x29E1A,0x29E34);
    makeRjt(0x29F28,0x29F32);
    makeRjt(0x2A240,0x2A254);
    makeRjt(0x2A84E,0x2A856);
    makeRjt(0x2A910,0x2A918);
    makeRjt(0x2A99E,0x2AA6A);
    makeRjt(0x2ACC8,0x2AD94);
    makeRjt(0x2B046,0x2B06A);
    makeRjt(0x2B2AC,0x2B378);
    makeRjt(0x2D268,0x2D366);
    makeRjt(0x2DC1E,0x2DC3E);
    makeRjt(0x2E200,0x2E226);
    makeRjt(0x2E23C,0x2E262);
    makeRjt(0x2E522,0x2E52A);
    makeRjt(0x2E882,0x2E888);
    makeRjt(0x2EAD6,0x2EADE);
    makeRjt(0x2EB7A,0x2EB82);
    makeRjt(0x2ED16,0x2ED1E);
    makeRjt(0x2F244,0x2F252);
    makeRjt(0x2F45E,0x2F466);
    makeRjt(0x2F49C,0x2F4A4);
    makeRjt(0x2F680,0x2F692);
    makeRjt(0x2F88A,0x2F892);
    makeRjt(0x2FB5C,0x2FB64);
    makeRjt(0x2FD1E,0x2FD26);
    makeRjt(0x2FED0,0x2FED8);
    makeRjt(0x3063E,0x3064E);
    makeRjt(0x307F0,0x30804);
}


static makeRjt(base,end){
    auto addr;
    
    addr = base;
    while(addr<end){
        MakeData(addr,FF_WORD,0x2,0);
        if(Word(addr)>0x8000){
            OpOffEx(addr,-1,REF_OFF32,base+Word(addr)-0x10000,base,0x10000);
        }else{
            OpOffEx(addr,-1,REF_OFF32,-1,base,0);
        }
        addr = addr+2;
    }
}


/*
 *    IDC SCRIPT : FIX INSTRUCTION REPRESENSATIONS
 *
 *     It is known that IDA's mc68 module doesn't
 *    properly output certain instructions with 
 *    specific addressing modes. 
 *
 *    This script helps to fix these 
 *    instructions :  
 *    move.w (loc,pc),(a1)+ were displayed as move.w loc,(a1)+. 
 *    move.l a6,usp were displayed as move a6,usp.
 *    jsr (loc).w were displayed as jsr loc.
 *    jsr loc(pc) were displayed as jsr loc.
 *    jmp (loc).w were displayed as jmp loc.
 *    jmp loc(pc) were displayed as jmp loc.  
 *    lea loc(pc),a0 were displayed as lea loc, a0.
 *    lea loc(pc),a1 were displayed as lea loc, a1. 
 *    lea loc(pc),a2 were displayed as lea loc, a2. 
 *    lea loc(pc),a3 were displayed as lea loc, a3. 
 *    lea loc(pc),a4 were displayed as lea loc, a4. 
 *    lea loc(pc),a5 were displayed as lea loc, a5. 
 *    lea loc(pc),a6 were displayed as lea loc, a6. 
 *    lea loc(pc),a7 were displayed as lea loc, a7.
 *
 *    It finds every instructions 
 *    in formatted code and asks for each one
 *    of them if you want it to be properly formatted.
 *
 *    "No" skips the instruction.
 *    "Cancel" stops the script.
 *
 *    Be aware that it will only find instructions if they have been formatted as code. 
 *    They won't be identified if they are still known as pure data. 
 *
 */
static fixInstructionRepresentations(){
    
    auto batch;
    
    Batch(0);
    //batch = AskYN(1,"Fix Instruction Represensations : BATCH MODE ?");
    //if (batch==-1) return;
    //Batch(batch);
    
    fix("32 FA","move.w  (%s,pc),(a1)+","move.w loc,(a1)+","move.w (loc,pc),(a1)+");
    fix("4E 66","move.l  a6,usp","move a6,usp","move.l a6,usp");
    
    fix("4E B8","jsr     (%s).w","jsr loc","jsr (loc).w");
    fix("4E BA","jsr     %s(pc)","jsr loc","jsr loc(pc)");
    fix("4E F8","jmp     (%s).w","jmp loc","jmp (loc).w");
    fix("4E FA","jmp     %s(pc)","jmp loc","jmp loc(pc)");
    
    fix("41 FA","lea     %s(pc), a0","lea loc, a0","lea loc(pc),a0");
    fix("43 FA","lea     %s(pc), a1","lea loc, a1","lea loc(pc),a1");
    fix("45 FA","lea     %s(pc), a2","lea loc, a2","lea loc(pc),a2");
    fix("47 FA","lea     %s(pc), a3","lea loc, a3","lea loc(pc),a3");
    fix("49 FA","lea     %s(pc), a4","lea loc, a4","lea loc(pc),a4");
    fix("4B FA","lea     %s(pc), a5","lea loc, a5","lea loc(pc),a5");
    fix("4D FA","lea     %s(pc), a6","lea loc, a6","lea loc(pc),a6");
    fix("4F FA","lea     %s(pc), a7","lea loc, a7","lea loc(pc),a7");

}


static fix(pattern, manualInstruction, wrongInstString, newInstString){

    auto addr;      // current location
    auto opnd;      // operand found at current location
    auto action;    // action asked to user
    auto rep;       // proper representation of instruction
    action = 1;
    
    // Start search from current address on screen
    for(addr=0;addr!=BADADDR;addr=FindBinary(addr+1,7,pattern)){
        opnd = GetOpnd(addr,0);
        if (opnd!=""){
            rep = form(manualInstruction,opnd);
            //Jump(addr);
            //action = AskYN(1,form("Change representation from %s to %s ?",wrongInstString, newInstString));
            if (action==-1) break;
            if (action==1){
                //Message(form("\n0x%d : %s changed to %s",addr,wrongInstString, newInstString));
                SetManualInsn(addr,rep);
            }
            else{
                //Message(form("\n0x%d : %s NOT changed to %s",addr,wrongInstString, newInstString));
            }
        }
    }
}


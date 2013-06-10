so2013
========
Para compilimit, duhet te ekzistoje cartela ./bin me file e inputit me te cilin marrin konfigurimi proceset

	20 sam 100 70 0 3
	50 genty 120 120 1 1
	80 elton 150 200 0 2

ky projekt funksionon ne ubuntu 13.04 per te tjera plataforma mbase duhen bere modifikime minimale..

per te kompiluar projektin:

	cd git/so2013
	make makefile all

kjo do te krijoje binaret ne direktoine : bin

per te eksekutuar programet hap 2 terminale e shkruaj:

	cd git/so2013/bin
	./gestore_coperanti

	ose/dhe

	./gestore_repository

tere kodi duhet te jete super i komentuar, ne menyre qe seli nga ne te marre vesh se cfare po ndodh e pse !


monada git per te bere commit nga ubuntu:

git commit -a -m "kompilim ne linux dhe gestore coperani funksional"
git push




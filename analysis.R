library(ggplot2)
responded<-read.csv("tld_cnt.result", header=FALSE)
asked<-read.csv("tld_cnt_all.input", header=FALSE)

for(tld in 1:length(asked$V1)){
  print(tld)
  for(i in 1:length(responded$V1)){
    if(strcmp(responded$V1[i], asked$V1[tld])){
      responded[i, 3] <- asked[tld, 2]
      asked[tld, 3] <- responded[i, 2]
    }
  }
}

density=15
x <- cbind(head(asked, density)$V2, head(asked, density)$V3)
barplot(t(x), las=2, col=c("darkblue", "red"), beside = T, 
          cex.names = 0.8, names.arg = head(asked, density)$V1, 
          ylim=c(0, 0.45), xlab="Domena najwy¿szego poziomu(TLD)", 
          ylab = "Czêœæ wszystkich domen", 
          main = "Zestawienie odpowiedzi ze wzglêdu\nna rozk³ad danych wejœciowych")
legend("topright", 
       legend = c("zapytania", "odpowiedzi"), 
       fill = c("darkblue", "red"), cex=0.8)
grid()

y <- cbind(head(responded, density)$V2, head(responded, density)$V3)
barplot(t(y), las=2, col=c("red", "darkblue"), beside = T, 
        cex.names = 0.8, names.arg = head(responded, density)$V1, 
        ylim=c(0, 0.45), xlab="Domena najwy¿szego poziomu(TLD)", 
        ylab = "Czêœæ wszystkich domen", 
        main = "Porównanie czêstoœci odpowiedzi\nwzglêdem danych wejœciowych")

legend("topright", 
       legend = c("zapytania", "odpowiedzi"), 
       fill = c("darkblue", "red"), cex=0.8)
grid()




input <-data.frame(c(1:length(asked$V2)), asked$V2)
for(a in 2:length(input$c))
{
  input[a, 2] <- (input[a-1,2]+input[a, 2])
}
plot(input, pch=" ", 
     main="Dystrybuanta TLD w domenach umo¿liwiaj¹cych AXFR",
     xlab="Lp. domeny najwy¿szego poziomu",
     ylab="CDF"
     )
lines(input)
grid(NULL, NULL, 6)

x=1
while(input[x, 2] < 0.99)
{
  print(x)
  x=x+1
}




countries<-read.csv("ipv6_country.result", header=FALSE, sep = ' ')
density=15
countries$V2<-countries$V2/sum(countries$V2)
x <- cbind(head(countries, density)$V2)
barplot(t(x), las=2, col=c("darkblue"), beside = T, 
        cex.names = 0.8, names.arg = head(countries, density)$V1, 
        ylim=c(0, 0.4), xlab="Kod pañstwa", 
        ylab = "Czêœæ wyodrêbnionych ASów", 
        main = "Adresy IPv6 (na podstawie AS)")
#legend("topright", 
#       legend = c("zapytania", "odpowiedzi"), 
#       fill = c("darkblue", "red"), cex=0.8)
grid()

size<-read.csv("new_zones_size_cnt.result", header=FALSE, sep = ',')




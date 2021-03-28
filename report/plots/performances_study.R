ht_index <- read.csv("~/Documents/Univ/Projets/INFO0027/source/MAGIC1/index.txt", header=FALSE)
ht_reset <- read.csv("~/Documents/Univ/Projets/INFO0027/source/MAGIC1/reset.txt", header=FALSE)
ht_gen <- ht_index + ht_reset

tst_index <- read.csv("~/Documents/Univ/Projets/INFO0027/source/MAGIC2/index.txt", header=FALSE)
tst_reset <- read.csv("~/Documents/Univ/Projets/INFO0027/source/MAGIC2/reset.txt", header=FALSE)
tst_gen <- tst_index + tst_reset

ht_nb_addresses <- seq(3000, 90000, by=3000)
tst_nb_adresses <- seq(1000, 30000, by=1000)

plot(ht_nb_addresses, ht_gen, type = "l", xlab="Number of addresses", ylab="Time (in seconds)", main="Hash table : total time", ylim=c(0, 0.11), xlim=c(0,90000), pch=15, col="blue")
plot(ht_nb_addresses, ht_index, type = "l", xlab="Number of addresses", ylab="Time (in seconds)", main="Hash table : MAGICindex time", ylim=c(0, 0.089), xlim=c(0,90000), pch=15, col="blue")
plot(ht_nb_addresses, ht_reset, type = "l", xlab="Number of addresses", ylab="Time (in seconds)", main="Hash table : MAGICReset time", ylim=c(0, 0.021), xlim=c(0,90000), pch=15, col="blue")

plot(tst_nb_adresses , tst_gen, type = "l", xlab="Number of addresses", ylab="Time (in seconds)", main="Ternary search trie : total time", ylim=c(0, 0.067), xlim=c(0,30000), pch=15, col="blue")
plot(tst_nb_adresses , tst_index, type = "l", xlab="Number of addresses", ylab="Time (in seconds)", main="Ternary search trie : MAGICindex time", ylim=c(0, 0.064), xlim=c(0,30000), pch=15, col="blue")
plot(tst_nb_adresses , tst_reset, type = "l", xlab="Number of addresses", ylab="Time (in seconds)", main="Ternary search trie : MAGICReset time", ylim=c(0, 0.0025), xlim=c(0,30000), pch=15, col="blue")

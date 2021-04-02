ht_index <- read.csv("~/Documents/Univ/Projets/INFO0027/source/MAGIC1/index.txt", header=FALSE)
ht_reset <- read.csv("~/Documents/Univ/Projets/INFO0027/source/MAGIC1/reset.txt", header=FALSE)
ht_gen <- ht_index + ht_reset

ht_mem <- read.csv("~/Documents/Univ/Projets/INFO0027/source/MAGIC1/mem.txt", header=FALSE)

tst_index <- read.csv("~/Documents/Univ/Projets/INFO0027/source/MAGIC2/index.txt", header=FALSE)
tst_reset <- read.csv("~/Documents/Univ/Projets/INFO0027/source/MAGIC2/reset.txt", header=FALSE)
tst_gen <- tst_index + tst_reset

tst_mem <- read.csv("~/Documents/Univ/Projets/INFO0027/source/MAGIC2/mem.txt", header=FALSE)

nb_addresses <- seq(500, 10000, by=500)

plot(nb_addresses, ht_gen, type = "l", xlab="Number of addresses", ylab="Time (in seconds)", main="Total time", ylim=c(0, max(tst_index) + max(tst_reset)), xlim=c(0,10000), pch=15, col="blue")
lines(nb_addresses, tst_gen, col = "red")
legend("topleft", legend = c("Hash table", "Ternary search trie"), col=c("blue", "red"), pch = c(15,15))

plot(nb_addresses, ht_index, type = "l", xlab="Number of addresses", ylab="Time (in seconds)", main="MAGICindex time", ylim=c(0, max(tst_index)), xlim=c(0,10000), pch=15, col="blue")
lines(nb_addresses, tst_index, col = "red")
legend("topleft", legend = c("Hash table", "Ternary search trie"), col=c("blue", "red"), pch = c(15,15))

plot(nb_addresses , ht_reset, type = "l", xlab="Number of addresses", ylab="Time (in seconds)", main="MAGICReset time", ylim=c(0, max(tst_reset)), xlim=c(0,10000), pch=15, col="blue")
lines(nb_addresses , tst_reset,  col = "red")
legend("topleft", legend = c("Hash table", "Ternary search trie"), col=c("blue", "red"), pch = c(15,15))

plot(nb_addresses , ht_mem, type = "l", xlab="Number of addresses", ylab="Size (in bytes)", main="Memory allocations", ylim=c(0, max(tst_mem)), xlim=c(0,10000), pch=15, col="blue")
lines(nb_addresses , tst_mem,  col = "red")
legend("topleft", legend = c("Hash table", "Ternary search trie"), col=c("blue", "red"), pch = c(15,15))



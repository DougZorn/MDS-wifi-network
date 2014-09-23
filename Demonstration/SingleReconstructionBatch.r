rawRead = read.csv("RSSIwrite.csv" , header = F)
rawRead <- rawRead [,-dim(rawRead)[2]] #removes nonsense data from terminal
rawRead <- rawRead [,-1]
n = dim(rawRead)[1]
distance = matrix(0, nrow=n, ncol=n)
coA = 1.518
coB = 0.270
StandarDev = 0.02798

truth = matrix(c(0, -2, 0, 0, 1, 0, -1, 1, 1, 1, -1, -1,2,2), nrow=7, ncol=2, byrow=T) # define on a per basis
rawRead <- .5*(rawRead + t(rawRead)) 
for(v in 1:n){
	for(w in 1:n){
		rawRead[v,w] <- if(rawRead[v,w]== 0) 0 else if(rawRead[v,w]>=128) ((rawRead[v,w]-256)/2-71) else (rawRead[v,w]/2-71)
	}
}
for(v in 1:n){
	for(w in 1:n){			
		distance[v,w] <- if(rawRead[v,w]== 0) 0 else 10^((log10(-rawRead[v,w])-coA)/coB)
	}
}


fit<-cmdscale(rawRead, eig = TRUE, k = 2)
X = fit$points
b = c(0, -2, 0, 0, 1, 0)
D = matrix(c(1, 0, X[1,1], X[1,2], 0, 0, 
          0, 1, 0, 0, X[1,1], X[1,2],
          1, 0, X[2,1], X[2,2], 0, 0,
          0, 1, 0, 0, X[2,1], X[2,2],
          1, 0, X[3,1], X[3,2], 0, 0,
          0, 1, 0, 0, X[3,1], X[3,2]), nrow=6, ncol=6, byrow=T)
cc = solve(D,b)
mu = c(cc[1], cc[2])
A = matrix( c(cc[3], cc[4], cc[5], cc[6]), nrow=2, ncol=2, byrow=T)
Z = X
for(i in 1: n){
  Z[i,] = mu + A%*%X[i,]
}

x11()
plot(Z[,1], Z[,2], xlab="feet", ylab="feet", main="Map", type="n",xlim=c(-10,10),ylim=c(-10,10))
points(truth[1:n,1],truth[1:n,2], col=1, pch=19, cex=1.5)
text(truth[1:n,1],truth[1:n,2],labels = row(fit$points), pos=4,cex=.75)
points(Z[,1],Z[,2], col=2, pch=20)
text(Z[,1],Z[,2],labels = row(fit$points), pos=4, col=2,cex=.75)
legend(2,4,legend=c("Truth", "Reconst"), pch=c(19,20), col=c(1,2))
Sys.sleep(5)
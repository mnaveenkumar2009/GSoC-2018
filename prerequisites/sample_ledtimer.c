//The following code is the sample_ledtimer.ceu program written in C
//for explanation of the example look at sample_ledtimer.ceu
#define f(i,n) for(i=0;i<n;i++)
#define AW150(j,i) analogWrite(j,i*150)
#define DR digitalRead
int a[4]={13,12,11,10},b[4]={6,7,8,9};
int i=0;
bool state[4]={0,0,0,0};
void PM(int x,bool y){
  if(y)
  pinMode(x,OUTPUT);
  else
  pinMode(x,INPUT);
}
void change(){
  int j,stateval=0;
  f(j,4)
  stateval+=(1<<j)*state[j];
  if(stateval==15){
    f(j,4)state[j]=0;
    f(j,4)
    AW150(b[j],state[j]);
    return;
  }
  stateval=(stateval+1)%16;
  
  j=0;
  while(stateval!=0){
    state[j++]=stateval%2;
    stateval/=2;
  }
  f(j,4)
  AW150(b[j],state[j]);
}
void set(){
  int j;
  f(j,4)
  AW150(a[j],0);
  if(i%5!=4)
  AW150(a[i%5],1);
}
void setup() {
PM(13,1);
PM(12,1);
PM(11,1);
PM(10,1);
PM(9,1);
PM(8,1);
PM(7,1);
PM(6,1);
PM(5,0);


}
void loop() {
  if(DR(5)){
      i=0;
      int j;
      f(j,4)
      {
        state[j]=0;
        AW150(a[j],0);
        AW150(b[j],0);
      }
  }
  delay(100);
  if(i%5==4)
  change();
  
  set();
  i++;
}
/*.........................................................................*/
// Importing necessary libraries
/*.........................................................................*/

#include<bits/stdc++.h>
using namespace std;

/*.........................................................................*/
//Basic Classes
/*.........................................................................*/ 


/* @Class Word
   @Made to contain a list of inputs from the user
   @Contains a vector of pairs and unsigned integer having no. of inputs
*/

class Word{
	
	//Bit *bits;
	vector<pair<char,int>>bits;
	unsigned int size;
	public:
	    void init_word();
	    void deinit_word();
	    friend void add_word(char operation,int address);
	    friend void simulate(char method);
};


/* @Class Block
   @Made to contain information of blocks in cache
   @Contains address of block,dirty_bit and last_used
*/

class Block{
	int address;
	int dirty_bit;
	int last_used;
	public:
		friend void init_cache();
		friend void deinit_cache();
		friend void reset_cache();
		friend int lru(char operation,int address,int word_num);
		friend void cache_word(int operation,int address, int word_num, char method);
		friend void show_cache();
};


/* @Class Set
   @Made to contain the information of sets in cache
   @Contains a pointer to Block class
*/

class Set{
	Block *blocks;
	public:
	    friend void init_cache();
	    friend void deinit_cache();
	    friend void reset_cache();
	    friend int lru(char operation,int address,int word_num);
	    friend void cache_word(int operation,int address, int word_num, char method);
	    friend void show_cache();
};


/* @Class Cache
   @Made to imitate Cache 
   @Contains a pointer to Set class and total no. of hits, misses, memory references and cache references
*/

class Cache{
	Set *sets;
	unsigned int hits;
	unsigned int misses;
	unsigned int memrefs;
	unsigned int cacherefs;
	public:
		void show_finalresult();
		friend void init_cache();
		friend void deinit_cache();
		friend void reset_cache();
		friend int lru(char operation,int address,int word_num);
		friend void cache_word(int operation,int address, int word_num, char method);
		friend void show_cache();
};

/*.........................................................................*/
//Global Variables
/*.........................................................................*/

Word *word;
Cache *cache;
unsigned int Block_size=0;
unsigned int Num_sets=0;
unsigned int Set_associativity=0;
unsigned int Word_bits=0;
unsigned int Index_bits=0;
unsigned int Tag_bits=0;
unsigned int Total_bits=0;

/*.........................................................................*/
// Required Functions
/*.........................................................................*/


/* @Function is_power_of2
   @Made to check if a number is power of 2 or not
*/

bool is_power_of_2(long int x)
{
    return (x != 0) && ((x & (x - 1)) == 0);
}


/* @Function init_word
   @Made to initialize word class
*/

void Word:: init_word()
{
	word=new Word;
	word->size=0;
}


/* @Function deinit_word
   @Made to deinitialize word class
*/

void Word:: deinit_word()
{
	delete word;
}


/* @Function add_word
   @Made to stor user inputs using vector
*/

void add_word(char operation,int address)
{
	word->size+=1;
	word->bits.push_back(make_pair(operation,address));
}


/* @Function init_cache
   @Made to initialize Cache
*/

void init_cache()
{
	cache= new Cache;
	cache->sets=new Set[Num_sets];
	for(int i=0;i<Num_sets;i++)
	{
		cache->sets[i].blocks= new Block[Set_associativity];
		for(int j=0;j<Set_associativity;j++)
		{
			cache->sets[i].blocks[j].address=-1;
			cache->sets[i].blocks[j].dirty_bit=0;
			cache->sets[i].blocks[j].last_used=0;
		}
	}
	
	cache->hits=0;
	cache->misses=0;
	cache->memrefs=0;
	cache->cacherefs=0;
}

/* @Function deinit_cache
   @Made to deinitialize Cache
*/

void deinit_cache()
{
    for(int i = 0; i < Num_sets; i++)
    {
        delete(cache->sets[i].blocks);
    }
    delete(cache->sets);
    delete(cache);
}


/* @Function reset_cache
   @Made to reset Cache
*/

void reset_cache()
{
	for(int i=0;i<Num_sets;i++)
	{
		for(int j=0;j<Set_associativity;j++)
		{
			cache->sets[i].blocks[j].address=-1;
			cache->sets[i].blocks[j].dirty_bit=0;
			cache->sets[i].blocks[j].last_used=0;
		}
	}
	
	cache->hits=0;
	cache->misses=0;
	cache->memrefs=0;
	cache->cacherefs=0;
}


/* @Function calculate_addressing_bits
   @Calculates addressing bits
*/

void calculate_addressing_bits(int memory_size,int cache_size, int block_size)
{
	Total_bits=log2(memory_size);
	Word_bits=log2(block_size);
	int Num_blocks=cache_size/block_size;
	Num_sets=Num_blocks/Set_associativity;
	Index_bits=log2(Num_sets);
	Block_size=block_size;
	Tag_bits=Total_bits-(Word_bits+Index_bits);
	//cout<<Total_bits<<" "<<Tag_bits<<" "<<Index_bits<<" "<<Word_bits<<endl;
}


/* @Function get_tagbits
   @Return Tag bits from an address
*/

unsigned int get_tagbits(unsigned int address)
{
    return address >> (Word_bits+Index_bits);
}
 
 
/* @Function get_indexbits
   @Return Index bits from an address
*/

unsigned int get_indexbits(unsigned int address)
{
    address = address >> Word_bits;
    int power=pow(2,Index_bits)-1;
    return address&power;
}


/* @Function lru
   @Gives the least recent used block
*/

int lru(char operation, int address,int word_num)
{
	int set_target=0;
	int replacement_set=0;
	int lru_bit=cache->sets[get_indexbits(address)].blocks[0].last_used;
	while(set_target<Set_associativity)
	{
		if(cache->sets[get_indexbits(address)].blocks[set_target].last_used<lru_bit)
		{
			lru_bit=cache->sets[get_indexbits(address)].blocks[set_target].last_used;
			replacement_set=set_target;
		}
		set_target+=1;
	}
	cache->sets[get_indexbits(address)].blocks[replacement_set].last_used=word_num;
	cache->sets[get_indexbits(address)].blocks[replacement_set].address=address;
	if(cache->sets[get_indexbits(address)].blocks[replacement_set].dirty_bit)
    {
        cache->memrefs += 1;
    }
    cache->sets[get_indexbits(address)].blocks[replacement_set].dirty_bit = 0;
    return replacement_set;
}


/* @Function gcache_word
   @Return the desired result according to the selected mode
*/

void cache_word(int operation,int address, int word_num, char method)
{
    cache->cacherefs += 1;
    int hit = -1;
    int free_space = -1;

    unsigned int set_target = 0;
    
    // Check for Existing Identical Tag to Update
    
    while(set_target < Set_associativity)
    {
        if(get_tagbits(cache->sets[get_indexbits(address)].blocks[set_target].address) == get_tagbits(address))
        {
            hit = set_target;
            cache->hits += 1;
            break;
        }
        set_target += 1;
    }
    
    // No Existing Identical Tag, Seek Empty Block
    
    if(hit<0)
    {
        cache->misses += 1;
        set_target = 0;
        while(set_target < Set_associativity)
        {
            if(cache->sets[get_indexbits(address)].blocks[set_target].address < 0)
            {
                free_space = set_target;
                break;
            }
            
            // Iterate to find a empty block in the cache
            
            set_target += 1;
        }
    }

    // ---------- Caching Policy ---------- //
    
    if(method == 'T')
    {
        // Write Through Rules
        
        if(operation == 'R'||operation=='r')
        {
            if(hit > -1)
            {
                // Read Hit
                cache->sets[get_indexbits(address)].blocks[hit].last_used = word_num;
            }
            else
            {
                // Read Miss
                cache->memrefs += 1;

                if(free_space > -1)
                {
                    cache->sets[get_indexbits(address)].blocks[free_space].last_used = word_num;
                    cache->sets[get_indexbits(address)].blocks[free_space].address = address;
                }
                else
                {
                    // LRU Replacement Policy
                    lru(operation,address,word_num);
                }
            }
        }
        else if(operation == 'W'||operation=='w')
        {
            if(hit > -1)
            {
                // Write Hit
                cache->memrefs += 1;
                cache->sets[get_indexbits(address)].blocks[hit].address = address;
                cache->sets[get_indexbits(address)].blocks[hit].last_used = word_num;
            }
            else
            {
                // Write Miss
                cache->memrefs += 1;
            }
        }
    }
    else if(method == 'B')
    {
        // Write Back Rules
        if(operation == 'R'||operation=='r')
        {
            if(hit > -1)
            {
                // Read Hit
                cache->sets[get_indexbits(address)].blocks[hit].last_used = word_num;
            }
            else
            {
                // Read Miss
                cache->memrefs += 1;
                if(free_space > -1)
                {
                    cache->sets[get_indexbits(address)].blocks[free_space].last_used = word_num;
                    cache->sets[get_indexbits(address)].blocks[free_space].address = address;
                }
                else
                {
                    // LRU Replacement Policy
                    lru(operation, address, word_num);
                }
            }
        }
        else if(operation == 'W'||operation=='w')
        {
            if(hit > -1)
            {
                // Write Hit
                cache->sets[get_indexbits(address)].blocks[hit].last_used = word_num;
                cache->sets[get_indexbits(address)].blocks[hit].address = address;
                cache->sets[get_indexbits(address)].blocks[hit].dirty_bit = 1;
            }
            else
            {
                // Write Miss
                cache->memrefs += 1;
                if(free_space > -1)
                {
                    cache->sets[get_indexbits(address)].blocks[free_space].last_used = word_num;
                    cache->sets[get_indexbits(address)].blocks[free_space].address = address;
                    cache->sets[get_indexbits(address)].blocks[free_space].dirty_bit = 1;
                }
                else
                {
                    // LRU Replacement Policy
                    int replacement_set = lru(operation,address, word_num);
                    cache->sets[get_indexbits(address)].blocks[replacement_set].dirty_bit = 1;
                }
            }
        }
    }
}


/* @Function show_cache
   @Made to show the current state of the cache
*/

void show_cache()
{
	int word=0;
	cout<<"\nCurent state of Cache is (Current Address[Last_used]Dirty Bit)\n\n";
    for(int i=0;i<Num_sets;i++)
    {
    	cout<<"Set "<<i<<endl;
        for(int j=0;j<Set_associativity;j++)
        {
        	cout<<"B"<<j<<" "<<"W"<<word<<"-"<<"W"<<word+Block_size-1<<" ";
        	cout<<cache->sets[i].blocks[j].address<<"["<<cache->sets[i].blocks[j].last_used<<"]"<<cache->sets[i].blocks[j].dirty_bit<<"    ";
        	word+=Block_size;
        }
        cout<<"\n";
    }
    cout<<endl;
}


/* @Function simulate
   @Made to simulate the process
*/

void simulate(char method)
{
	for(int i=0;i<word->size;i++)
	{
		if(method=='T'||method=='t')
		{
			cache_word(word->bits[i].first,word->bits[i].second,i+1,method);
		}
		else if(method=='B'||method=='b')
		{
			cache_word(word->bits[i].first,word->bits[i].second,i+1,method);
		}
	}
}


/* @Function show_finalresult
   @Returns the desired result
*/

void Cache:: show_finalresult()
{
	cout<<"Total number of Memory References: "<<cache->memrefs<<endl;
	cout<<"Total number of Cache References: "<<cache->cacherefs<<endl;
	cout<<"Total number of Cache Hits: "<<cache->hits<<endl;
	cout<<"Total number of Cache Misses: "<<cache->misses<<endl;
    cout<<"Hit Ratio: "<<float(cache->hits)/float(cache->hits+cache->misses)<<endl;
    cout<<"Miss Ratio: "<<float(cache->misses)/float(cache->hits+cache->misses)<<endl;
}

/*.........................................................................*/
//main code
/*.........................................................................*/

int main()
{
	cout<<"...........Welcome to MIPS Cache Stimulator..........\n";
	cout<<endl;
	cout<<"Enter the following fields(In power of 2)"<<endl;
	
	//Initialize word
	word->init_word();
	
	//Get Memory size, Block Size and Cache size
	int memory_size,block_size,cache_size;
	while(1)
	{
		cout<<"Enter cache size: ";
	    cin>>cache_size;
	    if(is_power_of_2(cache_size))
		{
			cout<<"Enter Memory size: ";
	        cin>>memory_size;
	        if(is_power_of_2(memory_size)&&(memory_size>cache_size))
		    {
		    	cout<<"Enter Block size: ";
	            cin>>block_size;
	            if(is_power_of_2(block_size)&&block_size+cache_size<=memory_size)
		        break;
		        else if(memory_size<cache_size+block_size)
		        cout<<"Memory Size is less than cache size and word size.\n";
		        else
		        cout<<"Block Size is not in power of 2.\n";	
			}
		    else if(memory_size<=cache_size)
		    cout<<"Memory Size should be greater than cache size.\n";
		    else
		    cout<<"Memory Size is not in power of 2.\n";	
		}
		else
		cout<<"Cache Size is not in power of 2\nPlease again enter cache size.\n";	
	}
	int exit=0;
	
	//Get Set Associativity
	cout<<"Enter the Set Associativity: ";
	{
		cin>>Set_associativity;
		while(exit!=1)
		{
		    if(cache_size/(block_size*Set_associativity)<=0||!is_power_of_2(Set_associativity))
		    {
		    	if(!is_power_of_2(Set_associativity))
		    	cout<<"Set associativity should be in power of 2\nPlease again enter Set Associativity"<<endl;
		    	else
			    cout<<"Invalid Configuration\nPlease again enter Set Associativity"<<endl;
			    cin>>Set_associativity;
		    }
		    else
		    exit=1;
		}
	}
	
	//Calculate Addressing Bits
	calculate_addressing_bits(memory_size,cache_size,block_size);
	
	//Initialize Cache
	init_cache();
	
	//Show Initial state of the Cache
	show_cache();
	
	//Get input for operations to be performed
	cout<<"Write operations to be performed along with the address."<<endl;
    exit=0;
	char op;
	int address;
	while(exit!=-1)
	{
		bool good=false;
		cout<<"Enter operation (R for read and W for write) and address in format: operation address)\n";
		cin>>op;
		cin>>address;
		if(address>=memory_size)
		{
			cout<<"Address out of memory size!\n";
			continue;
		}
		if(op=='R'||op=='r'||op=='W'||op=='w')
		{
			add_word(op,address);
			while(!good)
			{
				cout<<"Enter 1 to continue and -1 to exit"<<endl;
			    cin>>exit;
				if(!cin)
				{
					cout<<"Enter valid Number!"<<endl;
					cin.clear();
					cin.ignore(200,'\n');
				}
				else
				good=true;
			}
		}
		else
		cout<<"Enter a valid operation\n";
	}
	
	//Get input for type of caching policy
	int method;
	cout<<"Press 1 for Executing Write-Through with Write Allocate Cache Policy\nPress 2 for executing Write Back without Write Allocate Caching Policy\n";
	{
		cin>>method;
	    if(method==1)
	    {
	    	cout<<"Executing Write Through with Write allocate......"<<endl;
	    	simulate('T');
		}
	    else if(method==2)
		{
			cout<<"Executing Write Back without Write allocate......"<<endl;
			simulate('B');
		}
		else
		cout<<"Please enter valid Cache Policy\n";
	}
	
	//Show final state of the cache
	show_cache();
	
	//Show final result 
	cache->show_finalresult();
	
	//Deinitialize word
	word->deinit_word();
	
	//Denitialize cache
	deinit_cache();
	
	return 0;
}

/*.........................................................................*/
//end
/*.........................................................................*/

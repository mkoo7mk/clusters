/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <errno.h>

// gcc -std=c99 -Wall -Wextra -Werror -DNDEBUG cluster.c -o cluster -lm
// ./cluster SOUBOR 5

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap){
    assert(c != NULL);
    assert(cap >= 0);

    // TODO
    size_t size = sizeof(struct obj_t) * cap;
    void *arr = malloc(size);
    c->obj = (struct obj_t*)arr;
    c->size = 0;
    c->capacity = cap;

    if (arr == NULL)
        return;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c){
    // TODO
    if(c->capacity)
        free(c->obj);
    c->capacity = 0;
    c->size = 0;
    c->obj = NULL;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
    c = resize_cluster(c, c->size + CLUSTER_CHUNK);
    if (c == NULL)
        return;
    c->obj[c->size] = obj;
    c->size++;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
    for(int i = 0; i < c2->size; i++){
        append_cluster(c1, c2->obj[i]);
    }
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
    for (int i = idx; i <= narr - 1; i++){
        clear_cluster(&carr[i]);
        merge_clusters(&carr[i], &carr[i+1]);
    }
    clear_cluster(&carr[narr - 1]);
    return narr - 1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
    return sqrtf(powf(o1->x - o2->x, 2) + powf(o1->y-o2->y, 2)); // No need to use sqrtf but oh well I must
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);
    float min = INFINITY;
    float temp;
    for (int i = 0; i < c1->size; i++){
        for (int j = 0; j < c2->size; j++){
            temp = obj_distance(&c1->obj[i], &c2->obj[j]);
            if (temp < min)
                min = temp;
        }
    }
    return min;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
    float min = INFINITY;
    float temp;
    for (int i = 0; i < narr - 1; i++){
        for (int j = i + 1; j < narr; j++){
            temp = cluster_distance(&carr[i], &carr[j]);
            if (temp < min){
                min = temp;
                *c1 = i;
                *c2 = j;
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

int occurenceInString(char *haystack, char needle){
    int occurencies = 0;
    for (int i = 0; haystack[i] != '\0'; i++){
        if (haystack[i] == needle)
            occurencies++;
    }
    return occurencies;
}

int validateFile(char *filename){
    FILE *file;
    file = fopen(filename, "r");

    if (file == NULL)
        return -1;

    char str_buffer[20];
    while (fgets(str_buffer, 20, file) != NULL){
        if (strchr(str_buffer, '.') || (occurenceInString(str_buffer, ' ') != 2 && !strchr(str_buffer, '='))){
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

int uniqueID(struct cluster_t *arr, int arr_len, int id){
    for (int i = 0; i < arr_len; i++){
        if (arr[i].obj->id == id)
            return 0;
    }
    return 1;
}
/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    const int NUM_OF_OBJ_PARAMS = 3;
    // TODO

    int num_of_points = 0;
    char str_buffer[20];
    // char str_points_buffer[20];
    int loaded_params;
    // int i;
    FILE *file;
    struct obj_t temp_obj;
    // Open file
    file = fopen(filename, "r");
    if (file == NULL) {
        // Can not open file
        *arr = NULL;
        return -1;
    }
    // Read how many points in file
    fscanf(file, "%s", str_buffer);
    // Ask if it contains =
    char *temp = strchr(str_buffer, '=');
    if (temp == NULL){  // There is no =
        fclose(file);
        *arr = NULL;
        return -1;
    }
    num_of_points = atoi(temp + 1);

    if (num_of_points <= 0){ // If atoi does not return valid value
        *arr = NULL;
        fclose(file);
        return -1;
    }   
    *arr = malloc(sizeof(struct cluster_t) * num_of_points * CLUSTER_CHUNK);
    if (arr == NULL){ // No space for malloc
        fclose(file);
        return -1;
    }

    for (int i = 0; i < num_of_points; i++){
        loaded_params = fscanf(file, "%d %g %g", &temp_obj.id, &temp_obj.x, &temp_obj.y);
        if (loaded_params != NUM_OF_OBJ_PARAMS || temp_obj.x < 0 || temp_obj.x > 1000 || temp_obj.y < 0 || temp_obj.y > 1000 || temp_obj.id < 0 || !uniqueID(*arr, i, temp_obj.id)){
            *arr = NULL;
            fclose(file);
            return -1;
        }
        init_cluster(&(*arr)[i], 0);
        append_cluster(&(*arr)[i], temp_obj);
    }
    fclose(file);
    return num_of_points;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

/*
 Verifies arguments, sets desired number of clusters
 If failed then it sets the value to 0
*/
int check_arguments(int argc, char *argv[], int *desired_num_of_clusters){
    if (argc == 3){
        for (int i = 0; argv[2][i] != '\0'; i++){
            if (!isdigit(argv[2][i]))
                return 0;
        }
        *desired_num_of_clusters = atoi(argv[2]);
        if (*desired_num_of_clusters == 0)
            return 0;
        return 1;
    }
    if (argc == 2){
        *desired_num_of_clusters = 1;
        return 1;
    }
    *desired_num_of_clusters = 0;
    return 0;
}

int main(int argc, char *argv[]){
    struct cluster_t *clusters;
    int temp_c1_index, temp_c2_index;
    int desired_num_of_clusters = 0;
    float num_of_clusters;

    if(!check_arguments(argc, argv, &desired_num_of_clusters)){
        fprintf(stderr, "Invalid arguments\n");
        return -1;
    }

    if (!desired_num_of_clusters){
        fprintf(stderr, "Invalid arguments\n");
        return -1;
    }

    if (!validateFile(argv[1])){
        fprintf(stderr, "Wrong input file\n");
        return -1;
    }

    num_of_clusters = load_clusters(argv[1], &clusters);
    if (num_of_clusters < 1){
        fprintf(stderr, "Invalid arguments\n");
        for (int i = 0; i < num_of_clusters; i++){
            clear_cluster(&clusters[i]);
        }
        free(clusters);
        return -1;
    }
    
    while (num_of_clusters != desired_num_of_clusters){
        find_neighbours(clusters, num_of_clusters, &temp_c1_index, &temp_c2_index);
        merge_clusters(&clusters[temp_c1_index], &clusters[temp_c2_index]);
        num_of_clusters = remove_cluster(clusters, num_of_clusters, temp_c2_index);
    }
    print_clusters(clusters, num_of_clusters);
    for (int i = 0; i < num_of_clusters; i++){
        clear_cluster(&clusters[i]);
    }
    free(clusters);
    return 0;
}
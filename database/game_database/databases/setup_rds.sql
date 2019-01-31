create table Games (
    id              int             auto_increment,
    title           varchar(250)    not null,
    url             varchar(500)    not null,
    release_date    date            not null,
    metascore       int,
    user_score      float,
    num_user_ratings int,
    
    primary key (id),
    constraint unique (title, release_date)
);

create table GameGenre(
    game_id     int,
    genre_id    int,
    constraint primary key (game_id, genre_id)
);

alter table GameGenre
add foreign key (game_id) references Games(id);

create index genre_id_index
on GameGenre(genre_id);

create index release_date_index
on Games(release_date);





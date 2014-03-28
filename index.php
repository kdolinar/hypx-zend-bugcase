<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
  <head>
    <link rel="stylesheet" type="text/css" href="demo.css">
    <meta http-equiv="content-type" content="text/html; charset=UTF-8">
    <title>bugcase</title>
  </head>
  <body>
    <?php
        for ($i = 0; $i < 3; $i++) {
            $inq = HypxExpert::get()->makeInquiry(2);
            echo '<form action="#" method="post">';
            echo "HypxRadiosInquiry->getQuestion() = " . $inq->getQuestion();
            echo "HypxRadiosInquiry->getLabel(0) = " . $inq->getLabel(0);
            echo "HypxRadiosInquiry->getLabel(1) = " . $inq->getLabel(1);
            echo '<p>' . $inq->getQuestion() . '</p>';
            echo '<input type="radio" name="answer" value="' . $inq->getLabel(0) . '">' . $inq->getLabel(0) . '<br>';
            echo '<input type="radio" name="answer" value="' . $inq->getLabel(1) . '">' . $inq->getLabel(1) . '<br>';
            echo '<input type="submit" value="VREDU">';
            echo '</form>';
        }
    ?>
  </body>
</html>
